#include <string>
#include <vector>

#include "opts.hpp"
#include "midiReader.hpp"
#include "midifile.hpp"
#include "midiWriter.hpp"

void printHelp(const std::string& programName) {
	std::cout << "Usage: " << programName << " [options] <file>\n"
			  << "Options:\n"
			  << "  -d, --debug         Enable debug logging and print MIDI events\n"
			  << "  -n, --no-color      Disable colored output\n"
			  << "  -o, --output        Specify the name for the output file (defaults to \"out.mid\")\n"
			  << "  -r, --dryrun        Don't write anything to a file. Used for debugging purposes\n"
			  << "  -h, --help          Print this help message\n";
}

int main(int argc, char *argv[]) {

    opts::parse(argc, argv);

    if(opts::help == true) {
		printHelp(argv[0]);
		return 0;
	} 
	
	if(opts::debug == true) {
		Log::setLevel(Log::Debug);
	}
		
	if(opts::color == false){
		Log::enableColor(false);
	}

	if(opts::file.empty()) {
		Log::warn("No file specified");
		return 1;
	}

    MIDIReader mreader(opts::file);

    MIDIfile midifile = mreader.parseFile();

	midifile.mergeTracks();
	midifile.removeUnwantedEvents();
	midifile.removeChannels();
	midifile.fixNoteEvents();

	MIDIWriter mwriter(midifile);
	mwriter.writeFile();
	if(opts::dryrun == false) {
		mwriter.saveFile(opts::output_name);
	}

    return 0;

}