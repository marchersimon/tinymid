#include "log.hpp"

void Log::setLevel(const Level & level) {
	logLevel = level;
}

void Log::enableColor(const bool & c) {
	color = c;
}

void Log::status(const string & message) {
	std::cout << message << std::endl;
}

void Log::error(const string & message) {
	if(logLevel >= Error) {
		if(color) std::cout << "\033[;31m"; // red
		std::cout <<  message;
		if(color) std::cout << "\033[1;m";
		std::cout << std::endl;
	}
    throw std::runtime_error("");
}

void Log::warn(const string & message) {
	if(logLevel >= Warn) {
		if(color) std::cout << "\033[;33m"; // orange
		std::cout <<  message;
		if(color) std::cout << "\033[1;m";
		std::cout << std::endl;
	}
}

void Log::debug(const string & message) {
	if(logLevel >= Debug) {
		if(color) std::cout << "\033[;90m"; // grey
		std::cout <<  message;
		if(color) std::cout << "\033[1;m";
		std::cout << std::endl;
	}
}

string Log::to_hex_string(const uint32_t & num, bool prefix = true) {
	char s[9];
	sprintf(s, "%X", num);
	string str = s;
	if(str.size() % 2) {
		str.insert(0, "0");
	}
	if(prefix) {
		str.insert(0, "0x");
	}
	return str;
}


void Log::debugEvent(Event event, int startPos, int endPos, const vector<uint8_t> & file) {
	string line;

	line += addPadding(to_hex_string(startPos), 6);
	line += " | ";

	string content;
    for(int i = 0; i < endPos - startPos; i++) {
        content += Log::to_hex_string(file.at(startPos + i), false);
        content += " ";
    }
    if(content.length() > 39) {
        content.replace(27, content.length() - 34, "[...]");
    }
    line += addPadding(content, 39);

	line += "| ";
    line += addPadding(to_string(event.absoluteTime), 6);
    line += " | ";
    line += addPadding(to_string(event.deltaTime), 6);
    line += " | ";
    line += addPadding(MIDI::getEventName(event.type), 25);
    line += " | ";
    if(event.isMeta) {
        line += "          ";
    } else {
        line += addPadding("Channel " + to_string(event.channel), 10);
    }

    line += " | ";
    if(event.type == MIDI::event::NOTE_ON || event.type == MIDI::event::NOTE_OFF) {
        line += addPadding("Note " + MIDI::getNoteName(event.note), 9);
        line += "at velocity " + to_string(event.velocity);
    } else if(event.type == MIDI::event::TEMPO) {
        if(event.tempo % 1000000 == 0) {
            line += to_string(event.tempo / 1000000);
            line += " seconds";
        } else if(event.tempo % 1000 == 0) {
            line += to_string(event.tempo / 1000);
            line += " ms";
        } else {
            line += to_string(event.tempo);
            line += " us";
        }
        line += " per quarter note";
    } else if(event.type == MIDI::event::SEQUENCE_NAME) {
        line += event.seqName;
    } else if(event.type == MIDI::event::CONTROL_CHANGE) {
        if(event.ccdevice == MIDI::controler_message::PEDAL) {
            line += "Sustain Pedal";
            if(event.ccvalue < 64) {
                line += " off";
            } else {
                line += " on";
            }
        }
    }

    Log::debug(line);
}



void Log::debugEvent(Event event) {
	string line;

    line += addPadding(to_string(event.absoluteTime), 6);
    line += " | ";
    line += addPadding(to_string(event.deltaTime), 6);
    line += " | ";
    line += addPadding(MIDI::getEventName(event.type), 25);
    line += " | ";
    if(event.isMeta) {
        line += "          ";
    } else {
        line += addPadding("Channel " + to_string(event.channel), 10);
    }

    line += " | ";
    if(event.type == MIDI::event::NOTE_ON || event.type == MIDI::event::NOTE_OFF) {
        line += addPadding("Note " + MIDI::getNoteName(event.note), 9);
        line += "at velocity " + to_string(event.velocity);
    } else if(event.type == MIDI::event::TEMPO) {
        if(event.tempo % 1000000 == 0) {
            line += to_string(event.tempo / 1000000);
            line += " seconds";
        } else if(event.tempo % 1000 == 0) {
            line += to_string(event.tempo / 1000);
            line += " ms";
        } else {
            line += to_string(event.tempo);
            line += " us";
        }
        line += " per quarter note";
    } else if(event.type == MIDI::event::SEQUENCE_NAME) {
        line += event.seqName;
    } else if(event.type == MIDI::event::CONTROL_CHANGE) {
        if(event.ccdevice == MIDI::controler_message::PEDAL) {
            line += "Sustain Pedal";
            if(event.ccvalue < 64) {
                line += " off";
            } else {
                line += " on";
            }
        }
    }

    Log::debug(line);
}


string Log::addPadding(string str, int width) {
    if(width > str.size()) {
        str.insert(str.size(), width - str.size(), ' ');
	}
	return str;
}