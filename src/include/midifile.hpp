#pragma once

#include <string>
#include <vector>

#include "midi.hpp"

using std::string;
using std::vector;

struct Event {
    int deltaTime;
    int absoluteTime;
    MIDI::event type;

    bool isMeta = false;
    bool isSysex = false;

    int channel;
    int note;
    int velocity;
    int tempo;
    string seqName;
    int ccdevice; // controll change device
    int ccvalue;
};

class MIDIfile {
    public:
    void setFormat(int formatArg);
    void setNumberOfTracks(int numberOfTracksArg);
    int getNumberOfTracks();
    void setDivision(int divisionArg);
    void addTrack(vector<Event> track);

    private:
    vector<vector<Event>> tracks;
    int format;
    int numberOfTracks;
    int division;

};