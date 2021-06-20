#ifndef MIDFILE_H
#define MIDFILE_H

#include <iostream>
#include <fstream>

#include "log.h"

class Midfile {
	private:
		std::ifstream midstream;
		std::uint8_t *file;
		int size;
		int pos;
		int format;
		int numberOfTracks;
		int division;
		
		std::uint32_t getdword();
		std::uint16_t getword();
		std::uint8_t getbyte();
		int compareString(std::string s);

	public:
		Midfile(char filename[]);
		bool is_open();
		int read();
		int parseHeader();        
};

#endif // MIDFILE_H