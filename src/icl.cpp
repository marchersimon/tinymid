#include "icl.h"

void ICL::write(int number, int bytes) {
    if(number >= pow(2, bytes * 8)) {
        throw std::runtime_error("error");
    }

    for(int i = bytes - 1; i >= 0; i--) {
        file.push_back((number >> (i * 8)) & 0xFF);
    }
}

void ICL::write(string s, bool null_terminated) {
    for(const char & c : s) {
        file.push_back(c);
    }
    if(null_terminated) {
        file.push_back(0);
    }
}

void ICL::writeDeltaTime(int deltaTime) {
    vector<uint8_t> deltaTimeBytes;
    for(int i = 0; i < 4; i++) {

        if(i == 0) {
            deltaTimeBytes.insert(deltaTimeBytes.begin(), deltaTime & 0x7F);
            deltaTime = deltaTime >> 7;
        } else {
            deltaTimeBytes.insert(deltaTimeBytes.begin(), 0x80 | (deltaTime & 0x7F));
            deltaTime = deltaTime >> 7;
        }
        if(deltaTime <= 0) {
            break;
        }
    }
    file.insert(file.end(), deltaTimeBytes.begin(), deltaTimeBytes.end());
}

void ICL::createICL(const Midfile & midfile) {
    write("ICL", false);
    write(1, 2);
    write(maxNotes, 1);
    write(name);
    for(const Event & event : midfile.sortedEvents) {
        writeDeltaTime(event.delta);
        switch (event.type) {
        case event.NOTE_ON:
            write(0x00, 1);
            write(event.note, 1);
            break;
        case event.NOTE_OFF:
            write(0x10, 1);
            write(event.note, 1);
            break;
        case event.TEMPO:
            write(0x20, 1);
            write(event.tempo, 3);
        }
    }
}

void ICL::save() {
    Log::status("Output file name:");
    string path;
    std::cin >> path;
    std::ofstream iclstream(path, std::ofstream::binary);
    if(!iclstream.is_open()) {
        Log::error("Could not write to file " + path);
        throw std::runtime_error("error");
    }
    std::ostream_iterator<uint8_t> output_iterator(iclstream);
    std::copy(file.begin(), file.end(), output_iterator);
    iclstream.close();
}

void ICL::print() {
    for(const uint8_t & byte : file) {
        Log::debug(Log::to_hex_string(byte));
    }
}