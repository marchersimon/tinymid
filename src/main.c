#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>

extern int errno;

struct Mfile { // Midi-file
    int format;
    int trackChunks;
    int division;
};

int toInt(unsigned char *file, int start, int len) {
    if(sizeof(int) < len) {
        printf("ERROR: int not big enough to fit %d bytes\n", len);
    }
    int val = 0;
    for (int i = 0; i < len; i++) {
        val |= file[start + len - 1 - i] << (8 * i);
    }
    return val;
}

int cmpString(char *s1, char *s2, int s1Start) {
    for(int i = 0; s2[i]; i++) {
        if(s1[i + s1Start] != s2[i]) {
            return 1;
        }
    }
    return 0;
}

int parseTrack(char* track, int len) {
    if (len <= 0) {
        printf("Track length has to be one or greater\n");
        return 3;
    }

    if(cmpString(track, "MTrk", len) != 0) {
        printf("Track length does not correspond to actual length\n");
        return 3;
    }

    int trackIndex = 0;
    int iclIndex = 0;

    char icl[1000];

    for(int i = 0; i < 4; i++) {
        icl[iclIndex + i] = track[trackIndex + i];
        if((track[trackIndex + i] & 0x80 ) == 0) {
            trackIndex += i + 1;
            iclIndex += i + 1;
            break;
        }
        if(i == 3 && (track[i] & 0x80) != 0) {
            printf("Delta time cannot be greater than 4 Bytes\n");
            return 3;
        }
    }

}

int main(int argc, char *argv[]) {
    
    if(argc < 2) {
        printf("Please specify a file to convert\n");
        return 1;
    }

    FILE *fptr;

    fptr = fopen(argv[1], "r");

    if(fptr == NULL) {
      fprintf(stderr, "Error opening file: %s\n", strerror(errno));
      return 2;
    }

    fseek(fptr, 0, SEEK_END);
    int file_size = ftell(fptr);
    fseek(fptr, 0, SEEK_SET);
    unsigned char file[file_size];
    fread(file, file_size, 1, fptr);
    fclose(fptr);

    int index = 0;

    // Header

    if(cmpString(file, "MThd", 0) != 0) {
        printf("File is not a MIDI file\n");
        return 3;
    }
    index += 4;

    if(toInt(file, index, 4) != 6) {
        printf("Invalid header lenght\n");
        return 3;
    }
    index += 4;

    struct Mfile mfile;

    mfile.format = toInt(file, index, 2);
    index += 2;
    switch (mfile.format) {
        case 0:
            printf("Single track file format not supported yet\n");
            return 3;
        case 1:
            break;
        case 2:
            printf("Multiple song file format not supported yet\n");
            return 3;
    }

    mfile.trackChunks = toInt(file, index, 2);
    index += 2;
    if (mfile.trackChunks <= 0) {
        printf("File must have at least one track chunk\n");
        return 3;
    }

    mfile.division = toInt(file, index, 2);
    index += 2;
    if (mfile.division == 0) {
        printf("Division cannot be zero\n");
        return 3;
    } else if (mfile.division < 0) {
        printf("SMPTE compatible units not supported yet\n");
        return 3;
    }
    
    // Tracks

    if(cmpString(file, "MTrk", index) != 0) {
        printf("Expected \"MTrk\" at position 0x%02X\n", index);
        return 3;
    }
    index += 4;

    if(parseTrack(file + index + 4, toInt(file, index, 4)) != 0) {
        return 3;
    }

    return 0;
}