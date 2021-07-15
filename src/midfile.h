#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "log.h"
#include "event.h"

class Midfile {
	public:
		Midfile(std::string filename);
		int numberOfTracks;
		bool is_open();
		int read();
		int parseHeader();
		std::vector<Event> parseTrack();    
};