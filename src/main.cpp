#include <string>
#include <vector>

#include "opts.hpp"
#include "midiReader.hpp"
#include "midifile.hpp"
#include "midiWriter.hpp"

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

	midifile.mergeTracks();
	midifile.removeUnwantedEvents();
	midifile.removeChannels();
	midifile.fixNoteEvents();

	midifile.print();

	MIDIWriter mwriter(midifile);
	mwriter.writeFile();
	mwriter.saveFile();

    return 0;

}