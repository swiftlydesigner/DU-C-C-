//
//  main.c
//  Conway's Game of Life
//
//  Created by Kyle Parker on 5/26/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "game_of_life.h"
#include "arrays.h"
#include "utils.h"


// SHOULD_CHOOSE_RANDOM is used to determine if the cells should be spawned
// at random or not. Easy switch as grader has access to source code.
#define SHOULD_USE_RANDOM false

void usage(const char *prog) {
    fprintf(stderr,
            "Usage: %s <width> <height> <generations>\n"
            "  width        – number of columns\n"
            "  height       – number of rows\n"
            "  generations  – how many steps to simulate\n",
            prog);
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    int steps = atoi(argv[3]);

    if (width <= 0 || height <= 0 || steps < 0) {
        usage(argv[0]);
        return EXIT_FAILURE;
    }

    bool **grid = malloc_2d(width, height);
    
    if (!grid) {
        perror("malloc_2d");
        return EXIT_FAILURE;
    }

    /// TODO: Uncomment initialize_grid_random and comment initialize_grid_static for random

    if (SHOULD_USE_RANDOM) {
        initialize_grid_random(grid, width, height);
    } else {
        initialize_grid_static(grid, width, height);
    }

    for (int gen = 0; gen < steps; ++gen) {
        printf("Generation %d:\n", gen + 1);
        print_grid(grid, width, height);
        next_generation(grid, width, height);
    }

    free_2d(&grid, height);
    return EXIT_SUCCESS;
}
