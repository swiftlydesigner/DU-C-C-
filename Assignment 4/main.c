//
//  main.cpp
//  Assignment 4
//
//  Created by Kyle Parker on 5/18/26.
//



#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hw4.h"

int main(int argc, char *argv[]) {
    int status = EXIT_SUCCESS;

    if (argc < 2) {

        printUsage(argv[0]);
        status = EXIT_FAILURE;

    } else if (strcmp(argv[1], "encode") == 0) {

        status = encodeWithArgs(argc, argv);

    } else if (strcmp(argv[1], "decode") == 0) {

        status = decodeWithArgs(argc, argv);

    } else {

        fprintf(stderr, "Unknown command: %s\n\n", argv[1]);

        printUsage(argv[0]);

        status = EXIT_FAILURE;

    }

    return status;
}
