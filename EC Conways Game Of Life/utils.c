//
//  utils.c
//  Conway's Game of Life
//
//  Created by Kyle Parker on 5/26/26.
//

#include <stdlib.h>
#include <time.h>

#include "utils.h"

uint32_t getRandom(void) {
#if defined(ARC4RANDOM_AVAIL)
    return arc4random();
#else
    return rand();
#endif
}

void initialize_grid_random(bool **grid, int width, int height) {
    srand((unsigned)time(NULL));
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            grid[r][c] = (getRandom() % 4 == 0);
        }
    }
}

void initialize_grid_static(bool **grid, int width, int height) {
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            grid[r][c] = false;
        }
    }

    if (height > 3 && width > 3) {
        grid[1][2] = true;
        grid[2][3] = true;
        grid[3][1] = true;
        grid[3][2] = true;
        grid[3][3] = true;
    }
}

bool validRowColumn(int r, int c, int width, int height) {
    return r > 0      && c > 0
        && r < height && c < width;
}
