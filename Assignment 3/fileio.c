#include <stdio.h>
#include <stdlib.h>

#include "fileio.h"
#include "prng.h"

static void tryClose(FILE* file, const char * const msg) {
    int status = fclose(file);

    if (status != 0) {
        perror(msg);
    }
}

void save_random_numbers_to_file(const char *filename,
                                 const unsigned int *numbers,
                                 size_t count) {
    if (count == 0) {
        fprintf(stderr, "save_random_numbers_to_file: Nothing to write!");
        return;
    }

    FILE *file = fopen(filename, "wb");
    if (!file) {
        perror("Opening file for writing failed");
        return;
    }

    size_t written = fwrite(numbers, sizeof(unsigned int), count, file);

    if (written == 0) {
        perror("Failed to write anything");
    } else if (written != count) {
        perror("Failed complete write to binary");
    }

    tryClose(file, "Failed to close after write");
}


void load_random_numbers_from_file(const char *filename,
                                   unsigned int *numbers,
                                   size_t count) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file for read");
        return;
    }

    size_t read = fread(numbers, sizeof(unsigned int), count, file);

    if (read == 0) {
        perror("Failed to read anything");
    } else if (read != count) {
        perror("Failed to read compelte file");
    }

    tryClose(file, "Failed to close after read");
}

