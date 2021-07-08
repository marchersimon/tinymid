#pragma once

#include <getopt.h>
#include "log.h"

namespace opts {

	bool help = false;
	bool debug = false;
	bool color = true;
	std::string file;

	void parse(int argc, char *argv[]) {

		static struct option long_options[] = {
			{"help", no_argument, NULL, 'h'},
			{"debug", no_argument, NULL, 'd'},
			{"no-color", no_argument, NULL, 'n'},
			{NULL, 0, NULL, 0},
		};

		char opt;

		while((opt = getopt_long(argc, argv, "hdn", long_options, NULL)) != -1) {

			switch(opt) {
				case 'h':
					help = true;
					break;
				case 'd':
					debug = true;
					break;
				case 'n':
					color = false;
					break;
			}
		}

		if(optind < argc) { // if there are remaining arguments
			file = argv[optind];
		}
	}
}
