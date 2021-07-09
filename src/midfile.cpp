#include <iostream>
#include <fstream>
#include <vector>

#include "log.h"
#include "event.h"
#include "exception.h"

class Midfile {
	private:
		std::ifstream midstream;
		uint8_t *file;
		int size;
		int pos = 0;
		int format;
		int numberOfTracks;
		int division;

		uint32_t getdword();
		uint16_t getword();
		uint8_t getbyte();
		int getVariableLengthValue();
		Event getEvent(int offset);
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

uint8_t Midfile::getbyte() {
	return file[pos++];
}

uint16_t Midfile::getword() {
	return file[pos++] <<  8 |
		   file[pos++];
}

uint32_t Midfile::getdword() {
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
			Log::debug("File format 2: Multiple song file format");
			Log::error("Multiple song file format not supported");
			return 3;
		default:
			Log::error("Invalid file format: " + std::to_string(format));
			return 3;
	}

	// Check number of tracks
	numberOfTracks = getword();
	Log::debug("Number of tracks: " + std::to_string(numberOfTracks));
	if(numberOfTracks < 1) {
		Log::error("File has to contain at least one track");
		return 3;
	}

	
	division = (std::int16_t)getword(); // interpret uint16_t as int16_t
	Log::debug("Divisions: " + Log::to_hex_string(division) + ", " + std::to_string(division));
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
	pos += 4; // Ignore track length // TODO
	std::vector<Event> track;
	track.push_back(getEvent(0));
	while (track.back().type != 0x2F) {
		track.push_back(getEvent(track.back().totalTime));
	}
	return track;
}

Event Midfile::getEvent(int offset) {
	Event event;
	Log::debug("(" + Log::to_hex_string(pos) + ") new event:");

	event.delta = getVariableLengthValue();
	event.totalTime = offset + event.delta;

	Log::debug("    Delta time: " + std::to_string(event.delta));
	
	if(getbyte() == 0xFF) {
		Log::debug("    Meta event");
		event.meta = true;
		event.type = getbyte();
	} else {
		pos--;
		event.type = getbyte();
		if((event.type & 0xF0) != 0xF0) {
			Log::debug("    MIDI event on channel " + std::to_string(event.getChannel()));
			if(event.getChannel() != 0) {
				Log::debug("    Converting into channel 0");
				event.stripChannel();
			}
		} else {
			Log::debug("    MIDI event");
		}
	}

	Log::debug("    " + event.getEventName());

	int length;

	if(event.meta) {
		try {
			length = getVariableLengthValue();
		} catch (VLVException ex) {
			Log::error("(" + Log::to_hex_string(ex.getPos()) + ") Variable length value cannot be longer than 4 bytes");
		}
		if(event.getEventLength() == -1 || length == event.getEventLength()) {
			Log::debug("    Length: " + std::to_string(length) + " Bytes");
		} else {
			Log::error("Wrong meta event length: expected " + std::to_string(event.getEventLength()) + " Bytes, got " + std::to_string(length) + " Bytes");
		}
		switch(event.type) {
			case event.SEQUENCE_NUMBER:
			case event.TEXT_EVENT:
			case event.COPYRIGHT:
			case event.SEQUENCE_NAME:
			case event.INSTRUMENT:
			case event.LYRIC:
			case event.MARKER_TEXT:
			case event.CUE_POINT:
			case event.MIDI_CHANNEL_PREFIX:
			case event.END_OF_TRACK: // TODO
			case event.TEMPO: // TODO
			case event.SMPTE_OFFSET:
			case event.TIME_SIGNATURE: // TODO
			case event.KEY_SIGNATURE:
			case event.SEQUENCER_SPECIFIC:
				pos += length; // ignore data
				break;
		}
	} else {
		switch(event.type) {
			case event.NOTE_ON:
			case event.NOTE_OFF:
				event.note = getbyte();
				event.velocity = getbyte();
				Log::debug("    Note " + event.getNoteName() + " with velocity " + std::to_string(event.velocity)); 
				if(event.type == event.NOTE_ON && event.velocity == 0) {
					Log::debug("    Converting into note off");
					event.type = event.NOTE_OFF;
				}
				break;
			case event.KEY_PRESSURE:
			case event.CONTROLL_CHANGE:
			case event.PROGRAM_CHANGE:
			case event.CHANNEL_PRESSURE:
			case event.PITCH_WHEEL_CHANGE:
			case event.SYSTEM_MESSAGE:
				Log::debug("    Ignoring");
				pos += event.getEventLength();
				break;
		}
	}
	return event;
}

int Midfile::getVariableLengthValue() {
	int value = 0;
	for(int i = 0; i < 4; i++) {
		uint32_t byte = getbyte();
		value = (value << 7) | (byte & 0x7F);
		if(byte < 0x80) {
			break;
		}
		if(i == 4) {
			throw VLVException(pos - 4);
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