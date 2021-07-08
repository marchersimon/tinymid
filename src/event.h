#pragma once

class Event {
	public:
		std::uint32_t delta = 0;
		bool meta = false;
		std::uint8_t type;


		inline static std::uint8_t events[] = { // for Channel 0
			0x80, // Note off
			0x90, // Note on
			0xA0, // Polyphonic pressure
			0xB0, // Controll change
			0xC0, // Programm change
			0xD0, // Channel pressure
			0xE0, // Pitch wheel change
			0xF0, // System exclusive
		};

		static std::string getEventName(std::uint8_t event) {
			switch(event) {
				// MIDI
				case 0x80: return "Note off"; break;				
				case 0x90: return "Note on"; break;
				case 0xA0: return "Polyphonic key pressure"; break;
				case 0xB0: return "Controll change"; break;
				case 0xC0: return "Program change"; break;
				case 0xD0: return "Channel pressure"; break;
				case 0xE0: return "Pitch wheel change"; break;
				case 0xF0: return "System message"; break;				
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

		static int getEventLength(std::uint8_t event) {
			switch(event) {
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
};