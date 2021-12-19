#include "midifile.hpp"

void MIDIfile::setFormat(int formatArg) {
    format = formatArg;
}

void MIDIfile::setNumberOfTracks(int numberOfTracksArg) {
    numberOfTracks = numberOfTracksArg;
}

int MIDIfile::getNumberOfTracks() {
    return numberOfTracks;
}

void MIDIfile::setDivision(int divisionArg) {
    division = divisionArg;
}

void MIDIfile::addTrack(vector<Event> track) {
    tracks.push_back(track);
}