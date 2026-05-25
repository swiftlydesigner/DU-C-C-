#ifndef PW_GENERATOR_H
#define PW_GENERATOR_H

#include <stddef.h>

/*
 * Generates one password and stores it in out_password.
 *
 * Parameters:
 *   out_password   - buffer for the password string
 *   out_size       - total size of out_password buffer
 *   alphabet       - string of allowed characters
 *   alphabet_size  - number of usable characters in alphabet
 *   length         - desired password length
 *   random_numbers - array where raw random values used are stored
 *   random_index   - pointer to current insertion index in random_numbers
 *
 * Returns:
 *   1 on success, 0 on failure
 */
int generate_password(char *out_password,
                      size_t out_size,
                      const char *alphabet,
                      size_t alphabet_size,
                      int length,
                      unsigned int *random_numbers,
                      size_t *random_index);

int generate_n_passwords(size_t n,
                         char *out_password,
                         size_t out_size,
                         const char *alphabet,
                         size_t alphabet_size,
                         int length,
                         unsigned int *random_numbers,
                         size_t *random_index);

#endif
