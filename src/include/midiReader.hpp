#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <iterator>

#include "midifile.hpp"
#include "log.hpp"
#include "midi.hpp"

using std::string;
using std::vector;
using std::to_string;

class MIDIReader {
    public:
    MIDIReader(string filepath);
    MIDIfile parseFile();

    private:
    std::vector<uint8_t> file;
    uint pos = 0;

    uint8_t nextByte();
    uint16_t nextWord();
    uint32_t nextDWord();
    uint8_t prevByte();
    uint16_t prevWord();
    uint32_t prevDWord();
    void parseHeader(MIDIfile & midifile);
    vector<Event> parseTrack();
    Event parseEvent(const Event & previous, bool first = false);
    int parseVariableLengthValue();
};