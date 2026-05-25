#include <math.h> // log
#include <stdio.h> // printf
#include <string.h> // strlen

#include "information_content.h"

double information_content(int length, size_t alphabet_size) {
    if (length <= 0 || alphabet_size == 0) {
        return 0.0;
    }

    double log2n = log((double)alphabet_size) / log(2.0);

    return (double)length * log2n;
}

void displayInfoContentForPasswords(char** passwords, size_t num_passwords, char* alphabet) {
    printf("Using alphabet: %s\n", alphabet);

    size_t alphabet_size = strlen(alphabet);

    for (size_t i = 0; i < num_passwords; ++i) {
        char * pw = passwords[i];
        double ic = information_content(strlen(pw), alphabet_size);
        printf("Password %zu: %s\nInformation content %.2f bits\n\n", i + 1, pw, ic);
    }
}
