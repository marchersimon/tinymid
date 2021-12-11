#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "log.h"
#include "event.h"

using std::string;
using std::vector;

class Midfile {
	private:
		std::ifstream midstream;
		uint8_t *file;
		int size;
		int pos = 0;
		int format;

		uint32_t getdword();
		uint16_t getword();
		uint8_t getbyte();
		void rewind(const int & n = 1);
		int getVariableLengthValue();
		Event getEvent(const Event *previous);
		int compareString(const string & s);

	public:
		int division;
		Midfile(const string & filename);
		int numberOfTracks;
		vector<vector<Event>> tracks;
		vector<Event> sortedEvents;
		void mergeTracks();
		bool is_open();
		int read();
		int parseHeader();
		vector<Event> parseTrack();
		void filterEvents();
		string getSongName();
		void fixNoteEvents();
		int getNumberOfSimultaneousNotes();
};