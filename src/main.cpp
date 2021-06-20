#include <iostream>
#include <fstream>
#include <getopt.h>

#include "midfile.h"
#include "log.h"

void printHelp() {
	std::cout << "Usage: mid2icl [options] [file]\n"
				 "\n"
				 "Options:\n"
				 "  -h, --help   Display this help\n"
				 "  -d, --debug  Show debug messages\n" << std::endl;
}

int main(int argc, char *argv[]) {

	static struct option long_options[] = {
		{"help", no_argument, NULL, 'h'},
		{"debug", no_argument, NULL, 'd'},
		{NULL, 0, NULL, 0},
	};

	char opt;

	while((opt = getopt_long(argc, argv, "hd", long_options, NULL)) != -1) {
		switch(opt) {
			case 'h':
				printHelp();
				return 0;
			case 'd':
				log::setLevel("debug");
				break;
		}
	}

	if(argc < 2 || argv[argc - 1][0] == '-') {
		log::error("Please specify a file to convert");
		return 1;
	}

	Midfile midfile(argv[argc - 1]);
	if(!midfile.is_open()) {
		log::error((std::string)"File "  + argv[argc - 1] + " could not be opened");
		return 2;
	}
	log::debug((std::string)"Opened " + argv[argc - 1]);

	if(midfile.read()) {
		return 2;
	}

	if(midfile.parseHeader()) {
		return 3;
	}

	return 0;
}