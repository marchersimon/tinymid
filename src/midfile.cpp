#include <iostream>
#include <fstream>
#include <vector>

#include "log.h"
#include "event.h"

class Midfile {
	private:
		std::ifstream midstream;
		std::uint8_t *file;
		int size;
		int pos = 0;
		int format;
		int numberOfTracks;
		int division;

		std::uint32_t getdword();
		std::uint16_t getword();
		std::uint8_t getbyte();
		int getVariableLengthValue();
		Event getEvent();
		int compareString(std::string s);

	public:
		Midfile(std::string filename);
		bool is_open();
		int read();
		int parseHeader();
		std::vector<Event> parseTrack();
};

Midfile::Midfile(std::string filename) {
	midstream.open(filename, std::ios::binary);
}

std::uint8_t Midfile::getbyte() {
	return file[pos++];
}

std::uint16_t Midfile::getword() {
	return file[pos++] <<  8 |
		   file[pos++];
}

std::uint32_t Midfile::getdword() {
	return file[pos++] << 24 |
		   file[pos++] << 16 |
		   file[pos++] <<  8 |
		   file[pos++];
}

bool Midfile::is_open() {
	return midstream.is_open();
}

int Midfile::read() { // reads file into memory
	size = midstream.tellg();
	midstream.seekg(0, std::ios::end);
	size = (int)midstream.tellg() - size;

	if(size == 0) {
		Log::error("File cannot be empty");
		return 1;
	}
	file = (uint8_t*) malloc(size);
	if(file == NULL) {
		Log::error("Memory could not be allocated");
		return 1;
	}
	midstream.seekg(std::ios::beg);
	midstream.read((char*)file, size);

	midstream.close();
	Log::debug("File read into memory");
	return 0;
}

int Midfile::parseHeader() {
	// Check header string
	if(compareString("MThd")) {
		Log::error("File is not a valid MIDI file");
		return 3;
	}

	// Check header length
	if(getdword() != 6) {
		Log::error("Invalid header lenght");
		return 3;
	}

	// Check file format
	format = getword();
	switch(format) {
		case 0:
			Log::debug("File format 0: Single track file format");
			Log::error("Single track file format not supported yet");
			return 3;
		case 1:
			Log::debug("File format 1: Multiple track file format");
			break;
		case 2:
			Log::error("File format 2: Multiple fong file format");
			Log::error("Multiple song file format not supported");
			return 3;
		default:
			Log::error("Invalid file format: " + std::to_string(format));
			return 3;
	}

	// Check number of tracks
	numberOfTracks = getword();
	if(numberOfTracks < 1) {
		Log::debug("Number of tracks: " + std::to_string(numberOfTracks));
		Log::error("File has to contain at least one track");
		return 3;
	}

	
	division = (std::int16_t)getword(); // interpret uint16_t as int16_t
	Log::debug("Divisions: " + Log::hex_to_string(division) + ", " + std::to_string(division));
	if(division < 0) {
		Log::error("SMPTE compatible units not supported");
		return 3;
	} else if (division == 0) {
		Log::error("Division cannot be 0");
		return 3;
	}

	return 0;
}

std::vector<Event> Midfile::parseTrack() {
	if(compareString("MTrk")) {
		Log::error("Invalid header track");
		// exit program
	}
	pos += 4; // Ignore track length
	std::vector<Event> track;
	while(1) {
		track.push_back(getEvent());
		if(track.back().type == 0x2F) {
			break;
		}
	}
	return track;
}

Event Midfile::getEvent() {
	Event event;
	Log::debug("New event:");
	for(int i = 0; i < 4; i++) {
		std::uint8_t byte = getbyte();
		event.delta |= (byte << (8 * (4 - i)));
		if(byte < 0x80) {
			break;
		}
		if(i == 4) {
			Log::error("Delta time cannot be longer than 4 bytes");
			// exit program
		}
	}

	Log::debug("    Delta time: " + Log::hex_to_string(event.delta));

	if(getbyte() == 0xFF) {
		Log::debug("    Meta event");
		event.meta = true;
	} else {
		pos--;
		Log::debug("    MIDI event");
	}

	event.type = getbyte();

	Log::debug("    " + event.getEventName(event.type));

	int length;

	if(event.meta) {
		length = getVariableLengthValue();
		if(length == event.getEventLength(event.type)) {
			Log::debug("    Length: " + std::to_string(length) + " Bytes");
		} else {
			Log::error("Wrong meta event length: expected " + std::to_string(event.getEventLength(event.type)) + " Bytes, got " + std::to_string(length) + " Bytes");
		}
	}

	pos += length; //ignore data
	
	return event;
}

int Midfile::getVariableLengthValue() {
	int value = 0;
	for(int i = 0; i < 4; i++) {
		std::uint32_t byte = getbyte();
		value = value << (7 * (3 - i)) | (byte & 0x7F);
		if(byte < 0x80) {
			break;
		}
		if(i == 4) {
			Log::error("Variable length value cannot be longer than 4 bytes");
			// exit program
		}
	}
	return value;
}

int Midfile::compareString(std::string s) {
	
	for (char c : s) {
		if(c != file[pos++]) {
			return 1;
		}
	}
	
	return 0;
}