#pragma once

#include <string>

using std::string;

namespace MIDI {
    enum event {
			// MIDI
			NOTE_OFF = 0x80,
			NOTE_ON = 0x90,
			KEY_PRESSURE = 0xA0,
			CONTROL_CHANGE = 0xB0,
			PROGRAM_CHANGE = 0xC0,
			CHANNEL_PRESSURE = 0xD0,
			PITCH_WHEEL_CHANGE = 0xE0,
			SYSTEM_MESSAGE = 0xF0,
			// Meta
			SEQUENCE_NUMBER = 0x00,
			TEXT_EVENT = 0x01,
			COPYRIGHT = 0x02,
			SEQUENCE_NAME = 0x03,
			INSTRUMENT = 0x04,
			LYRIC = 0x05,
			MARKER_TEXT = 0x06,
			CUE_POINT = 0x07,
			MIDI_CHANNEL_PREFIX = 0x20,
			END_OF_TRACK = 0x2F,
			TEMPO = 0x51,
			SMPTE_OFFSET = 0x54,
			TIME_SIGNATURE = 0x58,
			KEY_SIGNATURE = 0x59,
			SEQUENCER_SPECIFIC = 0x7F,
		};

    enum controler_message {
		PEDAL = 0x40,
	};

    int getEventLength(event type);

    string getEventName(event type);

    string getNoteName(int note);
};