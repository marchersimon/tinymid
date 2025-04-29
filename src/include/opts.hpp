#pragma once

#include <string>

#include <getopt.h>

namespace opts {

	bool help = false;
	bool debug = false;
	bool color = true;
	bool dryrun = false;
	bool output_specified = false;
	std::string output_name = "out.mid";
	std::string file;

	void parse(int argc, char *argv[]) {

		static struct option long_options[] = {
			{"help", no_argument, NULL, 'h'},
			{"debug", no_argument, NULL, 'd'},
			{"no-color", no_argument, NULL, 'n'},
			{"dry-run", no_argument, NULL, 'r'},
			{"output", required_argument, NULL, 'o'},
			{NULL, 0, NULL, 0},
		};

		char opt;

		while((opt = getopt_long(argc, argv, "hdnro:", long_options, NULL)) != -1) {

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
				case 'r':
					dryrun = true;
					break;
				case 'o':
					output_name = optarg;
					output_specified = true;
					break;

			}
		}

		if(optind < argc) { // if there are remaining arguments
			file = argv[optind];
		}
	}
}