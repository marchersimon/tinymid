#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>

extern int errno;

int toInt(unsigned char *file, int start, int len) {
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

    if(strncmp(file, "MThd", 4)) {
        printf("File is not a MIDI file\n");
        return 3;
    }

    if(toInt(file, 4, 4) != 6) {
        printf("Invalid header lenght\n");
        return 3;
    }
    
    return 0;
}