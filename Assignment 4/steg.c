//
//  steg.cpp
//  Assignment 4
//
//  Created by Kyle Parker on 5/25/26.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "steg.h"
#include "ppm.h"
#include "utils.h"


unsigned char *readPayloadWithSize(const char *payloadPath, uint16_t *outSize) {
    FILE *f = fopen(payloadPath, "rb");

    if (f == NULL) {
        perror("fopen payload");
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        perror("fseek");
        fclose(f);
        return NULL;
    }

    long sizeLong = ftell(f);

    if (sizeLong < 0) {
        perror("ftell");
        fclose(f);
        return NULL;
    }

    if (sizeLong > 0xFFFF) {
        fprintf(stderr, "Error: payload too large (max 65535 bytes)\n");
        fclose(f);
        return NULL;
    }

    if (fseek(f, 0, SEEK_SET) != 0) {
        perror("fseek");
        fclose(f);
        return NULL;
    }

    uint16_t size16 = (uint16_t)sizeLong;
    unsigned char *buffer = malloc((size_t)sizeLong + 2);

    if (buffer == NULL) {
        perror("malloc");
        fclose(f);
        return NULL;
    }

    buffer[0] = (unsigned char)((size16 >> 8) & 0xFF);
    buffer[1] = (unsigned char)(size16 & 0xFF);

    if (sizeLong > 0) {
        size_t nread = fread(buffer + 2, 1, (size_t)sizeLong, f);

        if ((long)nread != sizeLong) {
            fprintf(stderr, "Error: could not read whole payload\n");
            free(buffer);
            fclose(f);
            return NULL;
        }
    }

    fclose(f);
    *outSize = size16;
    return buffer;
}

int encode(const char *inputPpm, const char *payloadPath, const char *outputPpm) {
    PPM *image = ppmRead(inputPpm);

    if (image == NULL) {
        fprintf(stderr, "Error: failed to read input PPM\n");
        return -1;
    }

    uint16_t payloadSize;
    unsigned char *payloadBuffer = readPayloadWithSize(payloadPath, &payloadSize);

    if (payloadBuffer == NULL) {
        ppmFree(image);
        return -1;
    }

    size_t totalPayloadBits = ((size_t)payloadSize + 2) * 8;
    size_t capacityBits = (size_t)image->width * image->height;

    if (totalPayloadBits > capacityBits) {
        fprintf(stderr, "Error: payload too large to fit in image (need %zu bits, have %zu bits)\n", totalPayloadBits, capacityBits);
        free(payloadBuffer);
        ppmFree(image);
        return -1;
    }

    size_t pixelCount = (size_t)image->width * image->height;
    size_t bitIndex = 0;

    for (size_t i = 0; i < pixelCount; ++i) {
        if (bitIndex >= totalPayloadBits) {
            break;
        }

        size_t base = i * 3;
        unsigned char *green = &image->data[base + 1];
        unsigned char *blue = &image->data[base + 2];

        size_t byteIndex = bitIndex / 8;
        int bitInByte = 7 - (int)(bitIndex % 8);
        int bit = (payloadBuffer[byteIndex] >> bitInByte) & 1;
        writeBit(green, blue, bit);
        bitIndex++;
    }

    if (!ppmWrite(outputPpm, image)) {
        fprintf(stderr, "Error: failed to write output PPM\n");
    }

    free(payloadBuffer);
    ppmFree(image);

    return 0;
}

int decode(const char *inputPpm, const char *outputFile) {
    PPM *image = ppmRead(inputPpm);
    if (image == NULL) {
        fprintf(stderr, "Error: failed to read input PPM\n");
        return -1;
    }

    size_t capacityBits = (size_t)image->width * image->height;
    if (capacityBits < 16) {
        fprintf(stderr, "Error: image too small to contain payload size\n");
        ppmFree(image);
        return -1;
    }

    size_t pixelCount = (size_t)image->width * image->height;
    uint16_t size16 = 0;
    size_t bitIndex = 0;
    size_t pixelIndex = 0;

    while (true) {
        if (pixelIndex >= pixelCount || bitIndex >= 16) {
            break;
        }

        size_t base = pixelIndex * 3;
        unsigned char green = image->data[base + 1];
        unsigned char blue = image->data[base + 2];
        int bit = readBit(green, blue);

        size16 = (uint16_t)((size16 << 1) | (bit & 1));
        bitIndex++;
        pixelIndex++;
    }

    size_t payloadBytes = (size_t)size16;
    size_t neededBits = payloadBytes * 8;
    size_t remainingBitsAvailable = 0;

    if (pixelCount > 16) {
        remainingBitsAvailable = pixelCount - 16;
    } else {
        remainingBitsAvailable = 0;
    }

    if (neededBits > remainingBitsAvailable) {
        fprintf(stderr, "Error: encoded size (%zu bytes) exceeds available bits (%zu bits)\n", payloadBytes, remainingBitsAvailable);
        ppmFree(image);
        return -1;
    }

    unsigned char *outBuffer = NULL;

    if (payloadBytes != 0) {
        outBuffer = malloc(payloadBytes);
        if (outBuffer == NULL) {
            perror("malloc");
            ppmFree(image);
            return -1;
        }
        memset(outBuffer, 0, payloadBytes);
    }

    size_t outBitIndex = 0;

    while (outBitIndex < neededBits) {
        size_t base = pixelIndex * 3;
        unsigned char green = image->data[base + 1];
        unsigned char blue = image->data[base + 2];
        int bit = readBit(green, blue);
        size_t byteIndex = outBitIndex / 8;
        int bitInByte = 7 - (int)(outBitIndex % 8);

        outBuffer[byteIndex] |= (unsigned char)(bit << bitInByte);
        outBitIndex++;
        pixelIndex++;
    }

    FILE *outFile = fopen(outputFile, "wb");

    if (outFile == NULL) {
        perror("fopen output");
        if (outBuffer != NULL) {
            free(outBuffer);
        }

        ppmFree(image);
        return -1;
    }
    if (payloadBytes > 0) {
        size_t written = fwrite(outBuffer, 1, payloadBytes, outFile);

        if (written != payloadBytes) {
            fprintf(stderr, "Error: short write to output\n");
        }
    }

    fclose(outFile);

    if (outBuffer != NULL) {
        free(outBuffer);
    }

    ppmFree(image);

    return 0;
}

int getLsb(unsigned char value) {
    return (int)(value & 1);
}

void setLsb(unsigned char *value, int bit) {
    *value = (unsigned char)((*value & ~1) | (bit & 1));
}

void writeBit(unsigned char *g, unsigned char *b, int bit) {
    int greenLsb = getLsb(*g);
    int blueLsb = getLsb(*b);
    int currentXor = greenLsb ^ blueLsb;

    if (currentXor == bit) {
        return;
    }

    *b ^= 1;
}

int readBit(unsigned char g, unsigned char b) {
    return getLsb(g) ^ getLsb(b);
}
