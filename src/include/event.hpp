#pragma once

#include <string>

#include "midi.hpp"

using std::string;

typedef struct {
    int deltaTime;
    int absoluteTime;
    MIDI::eventType type;

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