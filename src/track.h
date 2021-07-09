#pragma once

#include <vector>

#include "event.h"

class Track {
    private:
        std::vector<Event> track;
    public:
        Track(std::vector<Event> track) {
            this->track = track;
        }

    static std::vector<Event> merge(std::vector<Track> tracks) {
        // TODO
    }
};