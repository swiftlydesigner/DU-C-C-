//
//  ppm.hpp
//  Assignment 4
//
//  Created by Kyle Parker on 5/25/26.
//

#ifndef ppm_hpp
#define ppm_hpp

typedef struct {
    int width;
    int height;
    int maxval;
    unsigned char *data;
} PPM;

PPM *ppmRead(const char *path);
int ppmWrite(const char *path, const PPM *img);
void ppmFree(PPM *img);


#endif /* ppm_hpp */
