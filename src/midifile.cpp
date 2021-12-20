#include "midifile.hpp"

void MIDIfile::addTrack(vector<Event> track) {
    tracks.push_back(track);
}

void MIDIfile::mergeTracks() {
    vector<Event> mergedTrack;
    for(vector<Event> track : tracks) {
        mergedTrack.insert(mergedTrack.end(), track.begin(), track.end());
    }

    // stable_sort preserves the order between equal elements
    std::stable_sort(std::begin(mergedTrack), 
            std::end(mergedTrack), 
            [](Event a, Event b) {return a.absoluteTime < b.absoluteTime; });

    // the delta time isn't correct anymore, since it was relative to the previous event in that track
    mergedTrack[0].deltaTime = mergedTrack[0].absoluteTime;
    for(int i = 1; i < mergedTrack.size(); i++) {
        mergedTrack[i].deltaTime = mergedTrack[i].absoluteTime - mergedTrack[i-1].absoluteTime;
    }

    for(int i = mergedTrack.size() - 1; i >= 0; i--) {
        if(mergedTrack[i].type == MIDI::eventType::END_OF_TRACK) {
            mergedTrack.erase(mergedTrack.begin() + i);
        }
    }
    Event endOfTrack;
    endOfTrack.type = MIDI::eventType::END_OF_TRACK;
    endOfTrack.deltaTime = 0;
    endOfTrack.absoluteTime = mergedTrack.back().absoluteTime;
    mergedTrack.push_back(endOfTrack);

    numberOfTracks = 1;

    tracks.clear();
    tracks.push_back(mergedTrack);
}

void MIDIfile::removeUnwantedEvents() {
    for(int i = tracks.size() - 1; i >= 0; i--) {
        for(int k = tracks.at(i).size() - 1; k >= 0; k--) {
            switch (tracks.at(i).at(k).type) {
                case MIDI::eventType::NOTE_ON:
                case MIDI::eventType::NOTE_OFF:
                case MIDI::eventType::TEMPO:
                case MIDI::eventType::END_OF_TRACK:
                    continue;
            }
            tracks.at(i).erase(tracks.at(i).begin() + k);
        }
        if(tracks.at(i).size() == 0) {
            tracks.erase(tracks.begin() + i);
        }
    }
}

void MIDIfile::fixNoteEvents() {
    for(vector<Event> & track : tracks) {
        for(Event & event : track) {
            if(event.type == MIDI::eventType::NOTE_ON && event.velocity == 0) {
                event.type = MIDI::eventType::NOTE_OFF;
            }
        }
    }
}

void MIDIfile::removeChannels() {
    for(vector<Event> & track : tracks) {
        for(Event & event : track) {
            event.channel = 0;
        }
    }
}

void MIDIfile::print() {
    for(const vector<Event> & track : tracks) {
        Log::debug("Track:\n");
        for(const Event & event : track) {
           Log::debugEvent(event);
        }
    }
}