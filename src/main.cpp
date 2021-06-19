#include <iostream>
#include <fstream>

#include "midfile.h"

int main(int argc, char *argv[]) {

    if(argc < 2) {
        std::cout << "Please specify a file to convert" << std::endl;
        return 1;
    }

    Midfile midfile(argv[1]);
    if(!midfile.is_open()) {
        std::cout << "File " << argv[1] << " could not be opened" << std::endl;
        return 2;
    }

    if(midfile.read()) {
        return 2;
    }

    if(midfile.parseHeader()) {
        return 3;
    }

    return 0;
}