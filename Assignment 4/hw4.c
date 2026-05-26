//
//  hw4.c
//  Assignment 4
//
//  Created by main on 5/26/26.
//

#include <stdlib.h>

#include "hw4.h"
#include "utils.h"
#include "steg.h"

int encodeWithArgs(int argc, char **argv) {
    if (argc != 5) {
        fprintf(stderr, "Improper usage of encode! Aborting!\n\n");
        printEncodeUsage(argv[0]);
        return EXIT_FAILURE;
    }

    if (filesSame(argv[2], argv[3])
        || filesSame(argv[2], argv[4])
        || filesSame(argv[3], argv[4])) {

        fprintf(stderr, "Error: input, payload and output filenames must be distinct\n");
        return EXIT_FAILURE;
    }

    int encodeStatus = encode(argv[2], argv[3], argv[4]);

    if (encodeStatus == 0) {
        printf("Encode successful. Written to %s.\n\n", argv[4]);
    }

    return encodeStatus;
}

int decodeWithArgs(int argc, char **argv) {
    if (argc != 4) {
        fprintf(stderr, "Improper usage of decode! Aborting!\n\n");
        printDecodeUsage(argv[0]);
        return 1;
    }

    if (filesSame(argv[2], argv[3])) {
        fprintf(stderr, "Error: input and output filenames must be distinct\n");
        return 1;
    }

    int decodeStatus = decode(argv[2], argv[3]);

    if (decodeStatus == 0) {
        printf("Decode successful. Written to %s.\n\n", argv[3]);
    }

    return decodeStatus;
}

int printEncodeUsage(char *path) {
    return fprintf(stderr, "Encode:\n\t\t%s encode input.ppm payload.bin output.ppm\n", path);
}

int printDecodeUsage(char *path) {
    return fprintf(stderr, "Decode:\n\t\t  %s decode input.ppm output.bin\n", path);
}

int printUsage(char *path) {
    int charsWritten = fprintf(stderr, "Usage:\n\t");
    charsWritten += printEncodeUsage(path);
    putc('\t', stderr);
    charsWritten += printDecodeUsage(path);

    // ++ for the \t char
    return ++charsWritten;
}
