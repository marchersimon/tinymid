#include <iostream>
#include <fstream>
#include <getopt.h>

#include "midfile.h"

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
	};

	char opt;

	while((opt = getopt_long(argc, argv, "hd", long_options, NULL)) != -1) {
		switch(opt) {
			case 'h':
				printHelp();
				return 0;
			case 'd':
				// TODO
				break;
		}
	}

	if(argv[argc - 1][0] == '-') {
		std::cout << "Please specify a file to convert" << std::endl;
		return 1;
	}

	Midfile midfile(argv[argc - 1]);
	if(!midfile.is_open()) {
		std::cout << "File " << argv[argc - 1] << " could not be opened" << std::endl;
		return 2;
	}

	if(midfile.read()) {
		return 2;
	}

	if(midfile.parseHeader()) {
		return 3;
	}

	return 0;
}