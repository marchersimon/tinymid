#include <string>
#include <vector>

#include "opts.hpp"
#include "midiReader.hpp"
#include "midifile.hpp"

int main(int argc, char *argv[]) {

    opts::parse(argc, argv);

    if(opts::debug == true ) {
		Log::setLevel(Log::Debug);
	}
		
	if(opts::color == false){
		Log::enableColor(false);
	}

	if(opts::file.empty()) {
		Log::error("No file specified");
		return 1;
	}

    MIDIReader mreader(opts::file);

    MIDIfile midifile = mreader.parseFile();

    return 0;

}