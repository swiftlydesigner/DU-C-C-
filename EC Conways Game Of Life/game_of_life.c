//
//  game_of_life.c
//  Conway's Game of Life
//
//  Created by Kyle Parker on 5/26/26.
//

#include <stdbool.h>
#include <stdio.h>

#include "game_of_life.h"
#include "arrays.h"
#include "utils.h"

static int live_neighbours(bool **grid, int row, int col,
                           int width, int height) {
    int count = 0;

    for (int dr = -1; dr <= 1; ++dr) {
        for (int dc = -1; dc <= 1; ++dc) {
            if (dr == 0 && dc == 0) {
                continue;
            }

            int r = (row + dr);
            int c = (col + dc);

            // Treat off-grid values as dead
            if (validRowColumn(r, c, width, height)
                && grid[r][c]) {
                ++count;
            }
        }
    }

    return count;
}

// Check if the cell should live or die
bool game_of_life_rule(int row, int col, bool **grid,
                       int width, int height) {
    int numLiveNbrs = live_neighbours(grid, row, col, width, height);

    if (grid[row][col]) {
        return (numLiveNbrs == 2 || numLiveNbrs == 3);
    } else {
        return (numLiveNbrs == 3);
    }
}

void next_generation(bool **grid, int width, int height) {
    // Temp for entire grid
    bool **tmp = malloc_2d(width, height);

    if (!tmp) {
        perror("malloc");
        return;
    }

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            tmp[r][c] = game_of_life_rule(r, c, grid, width, height);
        }
    }

    // Update grid
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            grid[r][c] = tmp[r][c];
        }
    }

    free_2d(&tmp, height);
}


void print_grid(bool **grid, int width, int height)
{
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            putchar(grid[r][c] ? 'X' : '.');
        }
        putchar('\n');
    }
    putchar('\n');
}
