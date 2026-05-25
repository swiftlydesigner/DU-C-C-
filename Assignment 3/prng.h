#ifndef PRNG_H
#define PRNG_H

/*
 * Seeds the internal PRNG state.
 */
void prng_seed(unsigned int seed);

/*
 * Returns the next pseudo-random unsigned int.
 */
unsigned int prng_next(void);

/*
 * Single-step LCG function:
 * given a seed/current state, returns the next state.
 */
unsigned int lcg(unsigned int seed);

#endif
