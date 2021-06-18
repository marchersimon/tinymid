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

int main(int argc, char *argv[]) {

    // Check if a file has been passed
    
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

    if(strncmp(file, "MThd", 4) != 0) {
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
    
    return 0;
}