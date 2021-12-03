#pragma once

#include <iostream>
#include <vector>
#include <iterator>
#include <fstream>
#include <cmath>

#include "midfile.h"
#include "log.h"

using std::string;
using std::vector;

class ICL {
    private:
        vector<uint8_t> file;

    public:
        string name;
        int maxNotes;

        void write(int number, int bytes);
        void write(string s, bool null_terminated = true);
        void writeDeltaTime(int deltaTime);
        void createICL(const Midfile & midfile);
        void print();
        void save();
};