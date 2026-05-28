//
//  utils.h
//  Conway's Game of Life
//
//  Created by Kyle Parker on 5/26/26.
//

#ifndef utils_h
#define utils_h

#include <stdbool.h>
#include <stdlib.h>

#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)

#define ARC4RANDOM_AVAIL

#endif

uint32_t getRandom(void);

void initialize_grid_random(bool **grid, int width, int height);
void initialize_grid_static(bool **grid, int width, int height);

bool validRowColumn(int r, int c, int width, int height);

#endif /* utils_h */
