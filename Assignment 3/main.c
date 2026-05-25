#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "alphabet.h"
#include "pw_generator.h"
#include "information_content.h"
#include "prng.h"
#include "fileio.h"

static void print_usage(const char *prog_name) {
    fprintf(stderr,
            "Usage: %s length quantity [-luds] [alphabet]\n"
            "  length   Length of each password (positive integer)\n"
            "  quantity Number of passwords to generate (positive integer)\n"
            "  -luds    Optional flags selecting lowercase, uppercase, digits, symbols\n"
            "  alphabet Optional custom characters to include in the alphabet\n",
            prog_name);
}


int validateArguments(int argc, char **argv,
                      const char **customAlphabet,
                      const char **flagString,
                      int *pwLength,
                      size_t *pwQuantity) {
    *pwLength = atoi(argv[1]);
    if (*pwLength <= 0) {
        fprintf(stderr, "Error: length must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    *pwQuantity = (size_t)atoi(argv[2]);
    if (*pwQuantity == 0) {
        fprintf(stderr, "Error: quantity must be a positive integer.\n");
        return EXIT_FAILURE;
    }

    *flagString = NULL;
    *customAlphabet = NULL;

    for (int i = 3; i < argc; ++i) {
        if (argv[i][0] == '-') {
            *flagString = argv[i];
        } else {
            *customAlphabet = argv[i];
        }
    }
    return EXIT_SUCCESS;
}


/* Build the alphabet, allocate password storage and the raw
 * random‑number buffer.
 */
static int configAlphabetAndPwStorage(char *alphabet,
                                      int *alphabetLen,
                                      const char *customAlphabet,
                                      const char *flagString,
                                      int pwLength,
                                      size_t *pwBufLen,
                                      char **pwStorage,
                                      size_t pwQuantity,
                                      unsigned int **rndNumbers) {
    *alphabetLen = build_alphabet(flagString,
                                  customAlphabet,
                                  alphabet,
                                  sizeof(alphabet));
    if (*alphabetLen <= 0) {
        fprintf(stderr, "Error: failed to build a non‑empty alphabet.\n");
        return EXIT_FAILURE;
    }
    printf("Using alphabet: %s\n\n", alphabet);

    *pwBufLen = (size_t)pwLength + 1;
    *pwStorage = calloc(pwQuantity, *pwBufLen);
    if (!*pwStorage) {
        perror("calloc passwords");
        return EXIT_FAILURE;
    }

    size_t rndCap = pwQuantity * (size_t)pwLength * 4;
    *rndNumbers = malloc(rndCap * sizeof(unsigned int));
    if (!*rndNumbers) {
        perror("malloc rnd_numbers");
        free(*pwStorage);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

/* Write the generated random numbers and read them back,
 * checking that the first value matches.
 */
static int storeAndReadRandomNumbers(unsigned int **loaded,
                                     char **pwPtrArray,
                                     char *pwStorage,
                                     size_t rndIndex,
                                     unsigned int *rndNumbers) {
    const char *rndFile = "random_numbers.dat";

    save_random_numbers_to_file(rndFile, rndNumbers, rndIndex);
    printf("\nSaved %zu raw random values to '%s'.\n", rndIndex, rndFile);

    *loaded = malloc(rndIndex * sizeof(unsigned int));
    if (!*loaded) {
        perror("malloc loaded");
        free(pwPtrArray);
        free(pwStorage);
        free(rndNumbers);
        return EXIT_FAILURE;
    }

    load_random_numbers_from_file(rndFile, *loaded, rndIndex);
    printf("Loaded %zu values back from file.\n", rndIndex);
    if (rndIndex && rndNumbers[0] == (*loaded)[0]) {
        printf("First random value matches: %u\n", (*loaded)[0]);
    }

    return EXIT_SUCCESS;
}

/* Generate passwords, store their pointers, and display the
 * information‑content metrics.
 */
static int genPwsAndDisplay(char *alphabet,
                            int alphabetLen,
                            int pwLength,
                            size_t pwBufLen,
                            char ***pwPtrArray,
                            char *pwStorage,
                            size_t pwQuantity,
                            size_t *rndIndex,
                            unsigned int *rndNumbers) {
    if (!generate_n_passwords(pwQuantity,
                              pwStorage,
                              pwQuantity * pwBufLen,
                              alphabet,
                              (size_t)alphabetLen,
                              pwLength,
                              rndNumbers,
                              rndIndex)) {
        fprintf(stderr, "Error: password generation failed.\n");
        free(pwStorage);
        free(rndNumbers);
        return EXIT_FAILURE;
    }

    *pwPtrArray = malloc(pwQuantity * sizeof(char *));
    if (!*pwPtrArray) {
        perror("malloc pw_ptrs");
        free(pwStorage);
        free(rndNumbers);
        return EXIT_FAILURE;
    }

    for (size_t i = 0; i < pwQuantity; ++i) {
        (*pwPtrArray)[i] = pwStorage + i * pwBufLen;
    }

    displayInfoContentForPasswords(*pwPtrArray, pwQuantity, alphabet);
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) {
    if (argc < 3 || argc > 5) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    int pwLength;
    size_t pwQuantity;
    const char *flagString;
    const char *customAlphabet;

    if (validateArguments(argc, argv,
                          &customAlphabet,
                          &flagString,
                          &pwLength,
                          &pwQuantity) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    char alphabet[128];
    int alphabetLen;
    size_t pwBufLen;
    char *pwStorage;
    unsigned int *rndNumbers;
    size_t rndIndex = 0;

    if (configAlphabetAndPwStorage(alphabet,
                                   &alphabetLen,
                                   customAlphabet,
                                   flagString,
                                   pwLength,
                                   &pwBufLen,
                                   &pwStorage,
                                   pwQuantity,
                                   &rndNumbers) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    prng_seed((unsigned int)time(NULL));

    char **pwPtrArray;
    if (genPwsAndDisplay(alphabet,
                         alphabetLen,
                         pwLength,
                         pwBufLen,
                         &pwPtrArray,
                         pwStorage,
                         pwQuantity,
                         &rndIndex,
                         rndNumbers) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    unsigned int *loaded;
    if (storeAndReadRandomNumbers(&loaded,
                                  pwPtrArray,
                                  pwStorage,
                                  rndIndex,
                                  rndNumbers) == EXIT_FAILURE) {
        return EXIT_FAILURE;
    }

    free(loaded);
    free(pwPtrArray);
    free(pwStorage);
    free(rndNumbers);
    return EXIT_SUCCESS;
}
