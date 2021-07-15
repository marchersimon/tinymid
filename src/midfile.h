#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "log.h"
#include "event.h"

class Midfile {
	private:
		std::ifstream midstream;
		uint8_t *file;
		int size;
		int pos = 0;
		int format;
		int division;

		uint32_t getdword();
		uint16_t getword();
		uint8_t getbyte();
		int getVariableLengthValue();
		Event getEvent(Event *previous);
		int compareString(std::string s);

	public:
		Midfile(std::string filename);
		int numberOfTracks;
		bool is_open();
		int read();
		int parseHeader();
		std::vector<Event> parseTrack();
};