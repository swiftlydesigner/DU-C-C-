//
//  ppm.c
//  Assignment 4
//
//  Created by Kyle Parker on 5/25/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ppm.h"
#include "utils.h"
#include <stdbool.h>

// Parse the file and retrieve the next token.
static int nextToken(FILE *f, char *buf, size_t bufsz) {
    int ch;
    size_t pos = 0;
    
    while (true) {
        ch = fgetc(f);

        if (ch == EOF) {
            return 0;
        }

        if (isspace(ch) != 0) {
            continue;
        }

        if (ch == '#') {
            while (true) {
                ch = fgetc(f);
                if (ch == EOF || ch == '\n') {
                    break;
                }
            }
            continue;
        }


        // Replace the unused char back in the stream
        ungetc(ch, f);

        break;
    }

    while (pos + 1 < bufsz) {
        ch = fgetc(f);

        if (ch == EOF) {
            break;
        }

        if (isspace(ch)) {
            break;
        }
        
        buf[pos++] = (char)ch;
    }

    buf[pos] = '\0';

    return 1;
}

PPM *ppmRead(const char *path) {
    FILE *f = fopen(path, "r");

    if (f == NULL) {
        perror("fopen");
        return NULL;
    }

    char token[64];
    if (nextToken(f, token, sizeof(token)) == 0) {
        fclose(f);
        return NULL;
    }

    if (strcmp(token, "P3") != 0) {
        fprintf(stderr, "Error: unsupported PPM format (must be P3)\n");
        fclose(f);
        return NULL;
    }

    // Read width
    if (nextToken(f, token, sizeof(token)) == 0) {
        fprintf(stderr, "Error: truncated PPM header\n");
        fclose(f);
        return NULL;
    }

    int width = atoi(token);

    // Read height
    if (nextToken(f, token, sizeof(token)) == 0) {
        fprintf(stderr, "Error: truncated PPM header\n");
        fclose(f);
        return NULL;
    }

    int height = atoi(token);

    // Extract color value
    if (nextToken(f, token, sizeof(token)) == 0) {
        fprintf(stderr, "Error: truncated PPM header\n");
        fclose(f);
        return NULL;
    }

    int maxval = atoi(token);

    // Validate max color channel value is 255
    if (maxval != 255) {
        fprintf(stderr, "Error: only max color channel value 255 supported\n");
        fclose(f);
        return NULL;
    }

    // Validate dims
    if (width <= 0 || height <= 0) {
        fprintf(stderr, "Error: invalid image dimensions\n");
        fclose(f);
        return NULL;
    }

    size_t pixelCount = (size_t)width * (size_t)height;
    size_t components = pixelCount * 3;
    unsigned char *data = malloc(components);

    if (data == NULL) {
        perror("malloc");
        fclose(f);
        return NULL;
    }

    // Read components
    for (size_t i = 0; i < components; ++i) {
        if (nextToken(f, token, sizeof(token)) == 0) {
            fprintf(stderr, "Error: unexpected EOF while reading pixel data\n");
            free(data);
            fclose(f);
            return NULL;
        }

        int value = atoi(token);

        if (value < 0 || value > 255) {
            fprintf(stderr, "Error: pixel component out of range\n");
            free(data);
            fclose(f);
            return NULL;
        }

        data[i] = (unsigned char)value;
    }

    fclose(f);

    PPM *img = malloc(sizeof(PPM));

    if (img == NULL) {
        perror("malloc");
        free(data);
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->maxval = maxval;
    img->data = data;

    return img;
}

int ppmWrite(const char *path, const PPM *img) {
    FILE *f = fopen(path, "w");

    if (f == NULL) {
        perror("fopen");
        return 0;
    }

    // Encode image header with width, height, and max color channel value
    fprintf(f, "P3\n%d %d\n%d\n", img->width, img->height, img->maxval);

    size_t components = (size_t)img->width * img->height * 3;

    // Print the image details
    for (size_t i = 0; i < components; ++i) {
        fprintf(f, "%d", img->data[i]);

        if ((i + 1) % 12 == 0) {
            fprintf(f, "\n");
        } else {
            fprintf(f, " ");
        }
    }

    // Insert final \n char if DNE
    if (components % 12 != 0) {
        fprintf(f, "\n");
    }

    fclose(f);
    return 1;
}

void ppmFree(PPM *img) {

    if (img == NULL) {
        return;
    }

    if (img->data != NULL) {
        free(img->data);
    }

    free(img);
}
