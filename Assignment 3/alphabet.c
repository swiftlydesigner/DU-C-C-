#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "alphabet.h"

static const char *LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
static const char *UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const char *DIGITS    = "0123456789";
static const char *SYMBOLS   = "~!@#$%^&*()_+-={}[]|:;<>,.?/";

/* Adds characters from src into the used[] table. */
static void add_chars_to_set(const char *src, int used[128]) {
    if (src == NULL) {
        return;
    }

    for (size_t i = 0; src[i] != '\0'; i++) {
        unsigned char c = (unsigned char)src[i];
        if (c < 128) {
            used[c] = 1;
        }
    }
}

/* Validates that every character in src is graphical ASCII. */
static int validate_custom_alphabet(const char *src) {
    if (src == NULL) {
        return 1;
    }

    for (size_t i = 0; src[i] != '\0'; i++) {
        unsigned char c = (unsigned char)src[i];

        if (c >= 128 || !isgraph(c)) {
            fprintf(stderr, "Error: custom alphabet contains invalid character.\n");
            return 0;
        }
    }

    return 1;
}

/* Returns 1 if flags contains the given flag character, else 0. */
static int has_flag(const char *flags, char flag_char) {
    if (flags == NULL) {
        return 0;
    }

    /*
     * TODO:
     * Validate that flags starts with '-'
     * and only contains l, u, d, s after that.
     */

    if (flags[0] != '-') {
        return 0;
    }

    int found = 0;

    for (size_t i = 0;
         flags[i] != '\0' && found == 0;
         ++i) {
        if (flags[i] == flag_char) {
            found = 1;
        }
    }

    return found;
}

static int validate_flags(const char *flags) {
    if (flags == NULL) {
        return 1;
    }

    if (flags[0] != '-') {
        fprintf(stderr, "Error: flags must begin with '-'.\n");
        return 0;
    }

    for (size_t i = 1; flags[i] != '\0'; i++) {
        if (flags[i] != 'l' &&
            flags[i] != 'u' &&
            flags[i] != 'd' &&
            flags[i] != 's') {
            fprintf(stderr, "Error: invalid flag '%c'.\n", flags[i]);
            return 0;
        }
    }

    return 1;
}

static void appendAlphabet(const char *flags, int use_defaults, int *used) {
    if (use_defaults || has_flag(flags, 'l')) {
        add_chars_to_set(LOWERCASE, used);
    }
    
    if (use_defaults || has_flag(flags, 'u')) {
        add_chars_to_set(UPPERCASE, used);
    }
    
    if (use_defaults || has_flag(flags, 'd')) {
        add_chars_to_set(DIGITS, used);
    }
    
    if (use_defaults || has_flag(flags, 's')) {
        add_chars_to_set(SYMBOLS, used);
    }
}

//static void addStrToSet(const char * src, int used[128]) {
//    if (!src) {
//        return;
//    }
//
//    for (size_t i = 0; src[i] != '\0'; ++i) {
//        unsigned char c = (unsigned)src[i];
//
//        if (c < 128) {
//            // rm dupes
//            used[c] = 1;
//        } else {
//            fprintf(stderr, "WARNING: Ignoring character `%c`, outside of expected range.", c);
//        }
//    }
//}

static int writeToBuffer(size_t *index, char *out_alphabet, size_t out_size, int *used) {
    *index = 0;
    for (int c = 0; c < 128; ++c) {
        if (used[c]) {
            if (*index + 1 >= out_size) {
                fprintf(stderr, "Error: alphabet buffer too small.\n");
                return -1;
            }
            
            if (isgraph(c)) {
                out_alphabet[(*index)++] = (char)c;
            }
        }
    }
    return out_size;
}

int build_alphabet(const char *flags,
                   const char *custom_alphabet,
                   char *out_alphabet,
                   size_t out_size) {
    if (out_alphabet == NULL || out_size == 0) {
        return -1;
    }

    if (!validate_flags(flags)) {
        return -1;
    }

    if (!validate_custom_alphabet(custom_alphabet)) {
        return -1;
    }

    int used[128] = {0};

    /*
     * If the user gives no flags and no custom alphabet,
     * default to all built-in groups.
     */
    int use_defaults = (flags == NULL && custom_alphabet == NULL);

    add_chars_to_set(custom_alphabet, used);

    appendAlphabet(flags, use_defaults, used);

    size_t index = 0;
    writeToBuffer(&index, out_alphabet, out_size, used);

    out_alphabet[index] = '\0';
    return (int)index;
}
