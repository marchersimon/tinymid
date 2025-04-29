#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>
#include <math.h>

#include "midifile.hpp"

using std::string;
using std::vector;
using std::to_string;

class MIDIWriter {
    private:
    vector<uint8_t> file;
    MIDIfile & midifile;

    void writeHeader();
    void writeTracks();

    void writeString(string s);
    void writeBytes(int data, int bytes);
    void writeVLV(int data);

    public:
    MIDIWriter(MIDIfile & midifileArg);
    void writeFile();
    void saveFile(std::string output_file);
};