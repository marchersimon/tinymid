#include "midfile.h"

using std::string;
using std::vector;

Midfile::Midfile(const string & filename) {
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

// reads the file into a buffer
int Midfile::read() {
	size = midstream.tellg();
	midstream.seekg(0, std::ios::end);
	size = (int)midstream.tellg() - size;
	midstream.seekg(std::ios::beg);

	if(size == 0) {
		Log::error("File cannot be empty");
		return 1;
	}
	file = (uint8_t*) malloc(size);
	if(file == nullptr) {
		Log::error("Memory could not be allocated");
		return 1;
	}
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

	
	division = (int16_t)getword(); // interpret uint16_t as int16_t
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

vector<Event> Midfile::parseTrack() {
	if(compareString("MTrk")) {
		Log::error("Invalid header track");
		exit(EXIT_FAILURE);
	}
	pos += 4; // Ignore track length // TODO
	vector<Event> track;
	track.push_back(getEvent(nullptr));
	while (track.back().type != 0x2F) {
		track.push_back(getEvent(&track.back()));
	}
	return track;
}

Event Midfile::getEvent(const Event *previous) {
	Event event;
	int startPos = pos;

	event.delta = getVariableLengthValue();
	if(previous) {
		event.absoluteTime = previous->absoluteTime + event.delta;
	} else {
		event.absoluteTime = event.delta;
	}
	
	switch(int byte = getbyte()) {
		case 0xFF:
			event.meta = true;
			event.type = getbyte();
			break;
		case 0xF0:
			rewind();
			event.sysex = true;
			event.type = getbyte();
			break;
		default:
			if(byte < 0x80) {
				rewind();
				event.type = previous->type;
			} else {
				rewind();
				event.type = getbyte();
				event.stripChannel();
			}
	}

	int length;

	if(event.meta || event.sysex) {
		length = getVariableLengthValue();
		if(event.getEventLength() != -1 && length != event.getEventLength()) {
			Log::error("Wrong meta event length: expected " + std::to_string(event.getEventLength()) + " Bytes, got " + std::to_string(length) + " Bytes");
		}
		switch(event.type) {
			case event.TEMPO:
				event.tempo = (getword() << 8) | getbyte(); // get 3 bytes
				break;
			case event.SEQUENCE_NAME:
				for(int i = 0; i < length; i++) {
					event.name += (char)getbyte();
				}
				break;
			default:
				pos += length; // ignore data
		}
	} else {
		switch(event.type) {
			case event.NOTE_ON:
			case event.NOTE_OFF:
				event.note = getbyte();
				event.velocity = getbyte();
				if(event.type == event.NOTE_ON && event.velocity == 0) {
					event.type = event.NOTE_OFF;
				}
				break;
			case event.CONTROL_CHANGE:
				event.device = getbyte();
				event.value = getbyte();
				break;
			case event.KEY_PRESSURE:
			case event.PROGRAM_CHANGE:
			case event.CHANNEL_PRESSURE:
			case event.PITCH_WHEEL_CHANGE:
				pos += event.getEventLength(); // ignore data
				break;
		}
	}
	event.print(startPos, file, pos - startPos);
	return event;
}

void Midfile::rewind(const int & n) {
	pos -= n;
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
			Log::error("Variable length value cannot be longer than 4 bytes");
			exit(EXIT_FAILURE);
		}
	}
	return value;
}

int Midfile::compareString(const string & s) {
	
	for (const char & c : s) {
		if(c != file[pos++]) {
			return 1;
		}
	}
	
	return 0;
}