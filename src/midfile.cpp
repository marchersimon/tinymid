#include <iostream>
#include <fstream>

class Midfile {
    private:
        std::ifstream midstream;
        std::uint8_t *file;
        int size;
        int pos = 0;
        int format;
        int numberOfTracks;
        int division;

        std::uint32_t getdword();
        std::uint16_t getword();
        std::uint8_t getbyte();
        int compareString(std::string s);

    public:
        Midfile(char filename[]);
        bool is_open();
        int read();
        int parseHeader();        
};

Midfile::Midfile(char filename[]) {
    midstream.open(filename, std::ios::binary);
}

std::uint8_t Midfile::getbyte() {
    return file[pos++];
}

std::uint16_t Midfile::getword() {
    return file[pos++] <<  8 |
           file[pos++];
}

std::uint32_t Midfile::getdword() {
    return file[pos++] << 24 |
           file[pos++] << 16 |
           file[pos++] <<  8 |
           file[pos++];
}

bool Midfile::is_open() {
    return midstream.is_open();
}

int Midfile::read() {
    size = midstream.tellg();
    midstream.seekg(0, std::ios::end);
    size = (int)midstream.tellg() - size;

    if(size == 0) {
        std::cout << "File cannot be empty" << std::endl;
        return 1;
    }
    file = (uint8_t*) malloc(size);
    if(file == NULL) {
        std::cout << "Memory could not be allocated" << std::endl;
        return 1;
    }
    midstream.seekg(std::ios::beg);
    midstream.read((char*)file, size);

    midstream.close();
    return 0;
}

int Midfile::parseHeader() {
    if(compareString("MThd")) {
        std::cout << "File is not a valid MIDI file" << std::endl;
        return 3;
    }

    if(getdword() != 6) {
        std::cout << "Invalid header lenght" << std::endl;
        return 3;
    }

    format = getword();
    switch(format) {
        case 0:
            std::cout << "Single track file format not supported yet" << std::endl;
            return 3;
        case 1:
            break;
        case 2:
            std::cout << "Multiple song file format not supported yet" << std::endl;
            return 3;
        default:
            std::cout << "Invalid file format" << std::endl;
            return 3;
    }

    numberOfTracks = getword();
    if(numberOfTracks < 1) {
        std::cout << "File has to contain at least one track" << std::endl;
        return 3;
    }

    division = (std::int16_t)getword();
    if(division < 0) {
        std::cout << "SMPTE compatible units not supported yet" << std::endl;
        return 3;
    } else if (division == 0) {
        std::cout << "Division cannot be 0" << std::endl;
        return 3;
    }

    return 0;
}

int Midfile::compareString(std::string s) {
    
    for (char c : s) {
        if(c != file[pos++]) {
            return 1;
        }
    }
    
    return 0;
}