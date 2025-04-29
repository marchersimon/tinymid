#pragma once

#include <string>
#include <vector>
#include <algorithm>

#include "midi.hpp"
#include "log.hpp"
#include "event.hpp"

using std::string;
using std::vector;

class MIDIfile {
    public:
    vector<vector<Event>> tracks;
    int format;
    int numberOfTracks;
    int division;

    void addTrack(vector<Event> track);
    void mergeTracks();
    void removeUnwantedEvents();
    void fixNoteEvents();
    void removeChannels();
};