#pragma once

#include <iostream>

#include "log.h"

class Event {
	public:
		uint32_t delta = 0;
		int totalTime = 0;
		bool meta = false;
		bool sysex = false;
		uint8_t type;

		uint8_t note;
		uint8_t velocity;
		int tempo;

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

		std::string getEventName();
		int getEventLength();
		std::string getNoteName();
		void stripChannel();
		int getChannel();
		void print(int pos, uint8_t *file, int len);
		std::string formatColumn(std::string s, int width);
};