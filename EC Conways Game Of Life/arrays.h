//
//  arrays.h
//  Conway's Game of Life
//
//  Created by Kyle Parker on 5/26/26.
//

#ifndef arrays_h
#define arrays_h

#include <stdbool.h>

bool **malloc_2d(int width, int height);
void free_2d(bool ***grid, int height);

#endif /* arrays_h */
