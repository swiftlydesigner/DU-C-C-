//
//  utils.c
//  Assignment 4
//
//  Created by Kyle Parker on 5/25/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "utils.h"

// General Utils

int filesSame(const char *a, const char *b) {
    if (a == NULL || b == NULL) {
        return 0;
    }

    if (strcmp(a, b) == 0) {
        return 1;
    }

    struct stat statA;
    struct stat statB;

    if (stat(a, &statA) != 0) {
        return 0;
    }
    if (stat(b, &statB) != 0) {
        return 0;
    }

    if (statA.st_ino == statB.st_ino && statA.st_dev == statB.st_dev) {
        return 1;
    }

    return 0;
}

int fileExists(const char *path) {
    struct stat st;

    if (stat(path, &st) == 0) {
        return 1;
    }

    return 0;
}

void perrorExit(const char *msg) {
    perror(msg);
    exit(1);
}
