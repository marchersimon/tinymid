#include <iostream>
#include <fstream>

#include "midfile.h"
#include "log.h"
#include "opts.h"
#include "event.h"

void printHelp() {
	std::cout << "Usage: mid2icl [options] [file]\n"
				 "\n"
				 "Options:\n"
				 "  -h, --help       Display this help\n"
				 "  -d, --debug      Show debug messages\n"
				 "  -n, --no-color   No colors" << std::endl;
	exit(0);
}

int main(int argc, char *argv[]) {

	opts::parse(argc, argv);

	if(opts::help  == true ) printHelp();
	if(opts::debug == true ) Log::setLevel(Log::Debug);
	if(opts::color == false) Log::enableColor(false);

	if(opts::file.empty()) {
		Log::error("No file specified");
		return 1;
	}

	Midfile midfile(opts::file);
	if(midfile.is_open() == false) {
		Log::error("File "  + opts::file + " could not be opened");
		return 2;
	}
	Log::debug("Opened " + opts::file);

	if(midfile.read()) {
		return 2;
	}

	if(midfile.parseHeader()) {
		return 3;
	}

	Log::debug(std::string(133, '='));
	for(int i = 0; i < midfile.numberOfTracks; i++) {
		midfile.parseTrack();
		Log::debug(std::string(133, '='));
	}
	
	return 0;
}