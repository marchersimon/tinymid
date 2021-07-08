#pragma once

class Event {
	public:
		std::uint32_t delta = 0;
		bool meta = false;
		std::uint8_t type;

		std::uint8_t note;
		std::uint8_t velocity;

		enum event {
			NOTE_OFF = 0x80,
			NOTE_ON = 0x90,
			KEY_PRESSURE = 0xA0,
			CONTROLL_CHANGE = 0xB0,
			PROGRAM_CHANGE = 0xC0,
			CHANNEL_PRESSURE = 0xD0,
			PITCH_WHEEL_CHANGE = 0xE0,
			SYSTEM_MESSAGE = 0xF0,
		};

		std::string getEventName() {
			if((type & 0xF0) == 0xF0) {
				return "System message";
			}
			switch(type) {
				// MIDI
				case NOTE_OFF: return "Note off"; break;				
				case NOTE_ON: return "Note on"; break;
				case KEY_PRESSURE: return "Polyphonic key pressure"; break;
				case CONTROLL_CHANGE: return "Controll change"; break;
				case PROGRAM_CHANGE: return "Program change"; break;
				case CHANNEL_PRESSURE: return "Channel pressure"; break;
				case PITCH_WHEEL_CHANGE: return "Pitch wheel change"; break;
				// Meta
				case 0x00: return "Sequence number"; break;
				case 0x01: return "Text event"; break;
				case 0x02: return "Copyright notice"; break;
				case 0x03: return "Sequence or track name"; break;
				case 0x04: return "Instrument name"; break;
				case 0x05: return "Lyric text"; break;
				case 0x06: return "Marker text"; break;
				case 0x07: return "Cue point"; break;
				case 0x20: return "MIDI channel prefix assignment"; break;
				case 0x2F: return "End of track"; break;
				case 0x51: return "Tempo setting"; break;
				case 0x54: return "SMPTE offset"; break;
				case 0x58: return "Time signature"; break;
				case 0x59: return "Key signature"; break;
				case 0x7F: return "Sequencer specific event"; break;
			}
			return "Unknown event type";
		}

		int getEventLength() {
			switch(type) {
				case 0x00: return 2; break;
				case 0x20: return 1; break;
				case 0x2F: return 0; break;
				case 0x51: return 3; break;
				case 0x54: return 5; break;
				case 0x58: return 4; break;
				case 0x59: return 2; break;
			}
			return -1;
		}

		std::string getNoteName() {
			std::string noteName;
			switch(note % 12) {
				case 0: noteName.append("C"); break;
				case 1: noteName.append("C#"); break;
				case 2: noteName.append("D"); break;
				case 3: noteName.append("D#"); break;
				case 4: noteName.append("E"); break;
				case 5: noteName.append("F"); break;
				case 6: noteName.append("F#"); break;
				case 7: noteName.append("G"); break;
				case 8: noteName.append("G#"); break;
				case 9: noteName.append("A"); break;
				case 10: noteName.append("A#"); break;
				case 11: noteName.append("B"); break;
			}

			noteName += std::to_string((int)(note / 12) - 1);
			return noteName;
		}

		void stripChannel() {
			type &= 0xF0;
		}

		int getChannel() {
			return type & 0x0F;
		}
};