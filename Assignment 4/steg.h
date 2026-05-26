//
//  steg.hpp
//  Assignment 4
//
//  Created by Kyle Parker on 5/25/26.
//

#ifndef steg_hpp
#define steg_hpp

unsigned char *readPayloadWithSize(const char *payloadPath, uint16_t *outSize);

int encode(const char *inputPpm, const char *payloadPath, const char *outputPpm);
int decode(const char *inputPpm, const char *outputFile);

void setLsb(unsigned char *value, int bit);
int getLsb(unsigned char value);

void writeBit(unsigned char *g, unsigned char *b, int bit);
int readBit(unsigned char g, unsigned char b);

#endif /* steg_hpp */
