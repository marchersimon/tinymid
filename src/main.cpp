#include <iostream>
#include <fstream>

#include "midfile.h"
#include "log.h"
#include "opts.h"

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
	if(opts::debug == true ) log::setLevel("debug");
	if(opts::color == false) log::enableColor(false);

	if(opts::file.empty()) {
		log::error("No file specified");
		return 1;
	}

	Midfile midfile(opts::file);
	if(midfile.is_open() == false) {
		log::error("File "  + opts::file + " could not be opened");
		return 2;
	}
	log::debug("Opened " + opts::file);

	if(midfile.read()) {
		return 2;
	}

	if(midfile.parseHeader()) {
		return 3;
	}

	return 0;
}