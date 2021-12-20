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

void MIDIfile::mergeTracks() {
    vector<Event> mergedTrack;
    for(vector<Event> track : tracks) {
        mergedTrack.insert(mergedTrack.end(), track.begin(), track.end());
    }

    std::sort(std::begin(mergedTrack), 
            std::end(mergedTrack), 
            [](Event a, Event b) {return a.absoluteTime > b.absoluteTime; });

    tracks.clear();
    tracks.push_back(mergedTrack);
}

void MIDIfile::print() {
    for(const vector<Event> & track : tracks) {
        Log::debug("Track:\n");
        for(const Event & event : track) {
           Log::debugEvent(event);
        }
    }
}