#ifndef INFORMATION_CONTENT_H
#define INFORMATION_CONTENT_H

#include <stddef.h>

/*
 * Computes information content in bits using:
 *   length * log2(alphabet_size)
 */
double information_content(int length, size_t alphabet_size);

void displayInfoContentForPasswords(char** passwords, size_t num_passwords, char* alphabet);

#endif
