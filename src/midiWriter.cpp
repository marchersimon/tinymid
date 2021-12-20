#include "midiWriter.hpp"

MIDIWriter::MIDIWriter(MIDIfile & midifileArg) :midifile(midifileArg) {}

void MIDIWriter::writeFile() {
    writeHeader();
    writeTracks();
}

void MIDIWriter::writeHeader() {
    writeString("MThd");
    writeBytes(6, 4);
    writeBytes(0, 2);
    writeBytes(midifile.numberOfTracks, 2);
    writeBytes(midifile.division, 2);
}

void MIDIWriter::writeTracks() {
    for(const vector<Event> & track : midifile.tracks) {
        writeString("MTrk");
        // set the track size to zero for know and save the location
        int lengthPosition = file.size();
        writeBytes(0, 4);

        for(const Event & event : track) {
            writeVLV(event.deltaTime);

            if(event.type == MIDI::eventType::NOTE_ON || event.type == MIDI::eventType::NOTE_OFF) {
                writeBytes(event.type, 1);
                writeBytes(event.note, 1);
                writeBytes(event.velocity, 1);
            } else if(event.type == MIDI::eventType::TEMPO) {
                writeBytes(0xFF, 1);
                writeBytes(event.type, 1);
                writeBytes(0x03, 1);
                writeBytes(event.tempo, 3);
            } else if(event.type == MIDI::eventType::END_OF_TRACK) {
                writeBytes(0xFF, 1);
                writeBytes(event.type, 1);
                writeBytes(0, 1);
            }
        }

        // write actual length of track
        int trackLength = file.size() - (lengthPosition + 4);
        if(trackLength > pow(2, 32)) {
            Log::error("Cannot write " + to_string(trackLength) + " in only 4 Bytes of data");
        }

        for(int i = 3; i >= 0; i--) {
            file[lengthPosition + 3 - i] = ((trackLength >> (i * 8)) & 0xFF);
        }
    }
}

void MIDIWriter::writeString(string s) {
    for(const char & c : s) {
        file.push_back(c);
    }
}

void MIDIWriter::writeBytes(int data, int bytes) {
    if(data > pow(2, bytes * 8)) {
        Log::error("Cannot write " + to_string(data) + " in only " + to_string(bytes) + " Bytes of data");
    }

    for(int i = bytes - 1; i >= 0; i--) {
        file.push_back((data >> (i * 8)) & 0xFF);
    }
}

void MIDIWriter::writeVLV(int data) {
    if(data > pow(2, 28)) {
        Log::error("Variable length value cannot be greater than 28 bit");
    }
    if(data > pow(2, 21)) {
        file.push_back(((data >> 21) & 0x7F) | 0x80);
    }
    if(data > pow(2, 14)) {
        file.push_back(((data >> 14) & 0x7F) | 0x80);
    }
    if(data > pow(2, 7)) {
        file.push_back(((data >> 7) & 0x7F) | 0x80);
    }
    file.push_back(data & 0x7F);
}

void MIDIWriter::saveFile() {
    std::ofstream outstream("out.mid", std::ofstream::binary);
    if(outstream.is_open() == false) {
        Log::error("Could not open output stream");
    }
    outstream.write((const char*)file.data(), file.size());
}