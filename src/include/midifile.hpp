#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "midi.hpp"
#include "log.hpp"

using std::string;
using std::vector;

typedef struct {
    public:
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
} Event;

class MIDIfile {
    public:
    void setFormat(int formatArg);
    void setNumberOfTracks(int numberOfTracksArg);
    int getNumberOfTracks();
    void setDivision(int divisionArg);
    void addTrack(vector<Event> track);

    void mergeTracks();

    void print();

    private:
    vector<vector<Event>> tracks;
    int format;
    int numberOfTracks;
    int division;

};