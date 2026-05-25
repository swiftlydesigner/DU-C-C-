#include <stddef.h>

#include "pw_generator.h"
#include "prng.h"

/*
 * This helper should reduce modulo bias.
 *
 * A common strategy:
 *   - generate a random unsigned int
 *   - reject values above the largest multiple of alphabet_size
 *   - then use value % alphabet_size
 *
 * TODO:
 * Implement this fully.
 */

static unsigned int random_index_in_range(size_t alphabet_size) {
    if (alphabet_size == 0) {
        return 0;
    }

    // Use rejection sampling. Ensures all indicies are valid and in-range.

    const unsigned int max = 0xFFFFFFFF; // 32-bit
    const unsigned int threshold = max - (max % alphabet_size);

    unsigned int value = prng_next();

    while (value >= threshold) {
        value = prng_next();
    }

    return value % alphabet_size;
}

int generate_password(char *out_password,
                      size_t out_size,
                      const char *alphabet,
                      size_t alphabet_size,
                      int length,
                      unsigned int *random_numbers,
                      size_t *random_index) {
    
    if (out_password == NULL ||
            alphabet == NULL ||
            alphabet_size == 0 ||
            random_numbers == NULL ||
            random_index == NULL ||
            length <= 0 ||
            out_size < (size_t)length + 1) {
            return 0;
        }

    for (int i = 0; i < length; ++i) {
        unsigned int randIndex = random_index_in_range(alphabet_size);
        out_password[i] = alphabet[randIndex];
        random_numbers[*random_index] = randIndex;
        ++(*random_index);
    }

    out_password[length] = '\0';
    return 1;
}

int generate_n_passwords(size_t n,
                         char *out_password,
                         size_t out_size,
                         const char *alphabet,
                         size_t alphabet_size,
                         int length,
                         unsigned int *random_numbers,
                         size_t *random_index) {
    int status = 1;

    if (n == 0 ||
        out_password == NULL ||
        alphabet == NULL ||
        alphabet_size == 0 ||
        random_numbers == NULL ||
        random_index == NULL ||
        length <= 0) {
        status = 0;
    }

    size_t needed = n * ((size_t)length + 1);
    if (out_size < needed) {
        status = 0;
    }

    for (size_t i = 0; i < n && status; ++i) {
        char *dest = out_password + i * (length + 1);
        if (!generate_password(dest,
                               length + 1,
                               alphabet,
                               alphabet_size,
                               length,
                               random_numbers,
                               random_index)) {
            status = 0;
        }
    }

    return status;
}

