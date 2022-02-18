#include "randstate.h"

gmp_randstate_t state;

// Initializes a random state with a Mersenne Twister algorithm
// and uses seed as a random seed.
//
// seed: random seed
void randstate_init(uint64_t seed) {
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, seed);
}

// Clears and frees the memory used by the random state
void randstate_clear(void) {
    gmp_randclear(state);
}
