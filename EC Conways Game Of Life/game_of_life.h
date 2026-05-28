//
//  game_of_life.h
//  Conway's Game of Life
//
//  Created by Kyle Parker on 5/26/26.
//

#ifndef game_of_life_h
#define game_of_life_h

#include <stdbool.h>

bool game_of_life_rule(int row, int col, bool **grid,
                       int width, int height);

void next_generation(bool **grid, int width, int height);

void print_grid(bool **grid, int width, int height);

#endif /* game_of_life_h */
