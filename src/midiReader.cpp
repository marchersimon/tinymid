#include "midiReader.hpp"

MIDIReader::MIDIReader(string filepath) {

    // Open file stream
    std::ifstream instream;
    instream.open(filepath);
    if(instream.is_open() == false) {
        Log::error("File could not be opened");
    }

    // don't remove newlines
    instream.unsetf(std::ios::skipws);

    // read file stream to vector
    std::copy(std::istream_iterator<uint8_t>(instream), std::istream_iterator<uint8_t>(), std::back_inserter(file));

	instream.close();

	Log::debug("File \"" + filepath + "\" (" + to_string(file.size()) + " Bytes) successfully read into memory");
}

MIDIfile MIDIReader::parseFile() {
    MIDIfile midifile;
    parseHeader(midifile);
    Log::debug("\nInput File:");
    for(int i = 0; i < midifile.numberOfTracks; i++) {
        Log::debug("\nTrack " + to_string(i+1) + ":\n");
        Log::printTableHeader();
        midifile.addTrack(parseTrack());
    }
    return midifile;
}

void MIDIReader::parseHeader(MIDIfile & midifile) {
    if(nextDWord() != 0x4d546864 /*"MThd"*/) {
        Log::error("File does not start with \"MThd\"");
    }

    if(nextDWord() != 6) {
        Log::error("File has invalid lenght. Got " + to_string(prevDWord()) + ", expected 6");
    }

    int format = nextWord();
	switch(format) {
		case 0:
			Log::debug("File format 0: Single track file format");
			Log::error("Single track file format not supported yet");
		case 1:
			Log::debug("File format 1: Multiple track file format");
			break;
		case 2:
			Log::debug("File format 2: Multiple song file format");
			Log::error("Multiple song file format not supported");
		default:
			Log::error("Invalid file format: " + to_string(format));
	}
    midifile.format = format;

    int numberOfTracks = nextWord();
    midifile.numberOfTracks = numberOfTracks;
    
    int division = nextWord();
    midifile.division = division;
}

vector<Event> MIDIReader::parseTrack() {

    if(nextDWord() != 0x4d54726b /*"MTrk"*/) {
        Log::error("Track does not start with \"MTrk\"");
    }

    int trackLength = nextDWord();

    vector<Event> track;
    track.push_back(parseEvent(Event(), true));
    while(track.back().type != MIDI::END_OF_TRACK) {
        track.push_back(parseEvent(track.back()));
    }
    return track;
}

Event MIDIReader::parseEvent(const Event & previous, bool first) {
    Event event;

    int startPos = pos;
    event.deltaTime = parseVariableLengthValue();
    if(first) {
        event.absoluteTime = 0;
    } else {
        event.absoluteTime = event.deltaTime + previous.absoluteTime;
    }

    int byte = nextByte();

    if(byte < 0x80) {
        pos--;
        event.isMeta = previous.isMeta;
        event.isSysex = previous.isSysex;
        event.type = previous.type;
        if(event.isSysex || event.isMeta) {
            nextByte();
        }
    } else {
        switch(byte) {
            case 0xFF:
                event.isMeta = true;
                event.type = (MIDI::eventType)nextByte();
                break;
            case 0xF0:
                event.isSysex = true;
                event.type = (MIDI::eventType)nextByte();
                break;
            default:
                event.type = (MIDI::eventType)(prevByte() & 0xF0);
                event.channel = (prevByte() & 0x0F);
        }
    }


	if(event.isMeta || event.isSysex) {
        int eventLength;
		eventLength = parseVariableLengthValue();
		if(MIDI::getEventLength(event.type) != -1 && eventLength != MIDI::getEventLength(event.type)) {
			Log::error("Wrong meta event length: expected " + std::to_string(MIDI::getEventLength(event.type)) + " Bytes, got " + std::to_string(eventLength) + " Bytes");
		}
		switch(event.type) {
			case MIDI::eventType::TEMPO:
				event.tempo = (nextWord() << 8) | nextByte(); // get 3 bytes
				break;
			case MIDI::eventType::SEQUENCE_NAME:
				for(int i = 0; i < eventLength; i++) {
					event.seqName += (char)nextByte();
				}
				break;
			default:
				pos += eventLength; // ignore data
		}
	} else {
		switch(event.type) {
			case MIDI::eventType::NOTE_ON:
			case MIDI::eventType::NOTE_OFF:
				event.note = nextByte();
				event.velocity = nextByte();
				break;
			case MIDI::eventType::CONTROL_CHANGE:
				event.ccdevice = nextByte();
				event.ccvalue = nextByte();
				break;
			case MIDI::eventType::KEY_PRESSURE:
			case MIDI::eventType::PROGRAM_CHANGE:
			case MIDI::eventType::CHANNEL_PRESSURE:
			case MIDI::eventType::PITCH_WHEEL_CHANGE:
				pos += getEventLength(event.type); // ignore data
				break;
		}
	}

    Log::debugEvent(event, startPos, pos, file);
    return event;
}

int MIDIReader::parseVariableLengthValue() {
    int value = 0;
    for(int i = 0; i < 4; i++) {
        uint32_t byte = nextByte();
        value = (value << 7) | (byte & 0x7F);
        if(byte < 0x80) {
            break;
        }
        if(i == 4) {
            Log::error("Variable length value cannot be longer than 4 bytes");
        }
    }
    return value;
}

uint8_t MIDIReader::nextByte() {
    return file.at(pos++);
}

uint16_t MIDIReader::nextWord() {
    return (file.at(pos++) << 8) |
            file.at(pos++);
}

uint32_t MIDIReader::nextDWord() {
    return (file.at(pos++) << 24) | 
           (file.at(pos++) << 16) | 
           (file.at(pos++) <<  8) | 
            file.at(pos++);
}

uint8_t MIDIReader::prevByte() {
    pos--;
    return nextByte();
}

uint16_t MIDIReader::prevWord() {
    pos -= 2;
    return nextWord();
}

uint32_t MIDIReader::prevDWord() {
    pos -= 4;
    return nextDWord();
}