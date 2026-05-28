//
//  arrays.c
//  Conway's Game of Life
//
//  Created by Kyle Parker on 5/26/26.
//

#include <stdlib.h>

#include "arrays.h"

bool **malloc_2d(int width, int height) {
    bool success = true;

    bool **grid = malloc(height * sizeof(bool *));

    if (!grid) {
        return NULL;
    }

    int i = 0;

    for (i; i < height && success; ++i) {
        bool *data = calloc(width, sizeof(bool));

        if (!data) {
            while (--i >= 0) {
                free(grid[i]);
            }
            
            success = false;
        }

        grid[i] = data;
    }

    if (!success) {
        free(grid);
        return NULL;
    }

    return grid;
}


void free_2d(bool ***grid, int height) {
    if (!grid || !*grid) {
        return;
    }

    bool** p = *grid;

    for (int i = 0; i < height; ++i) {
        free(p[i]);
    }

    free(p);

    *grid = NULL;
}
