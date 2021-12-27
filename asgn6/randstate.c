#include <stdio.h>
#include <stdint.h>
#include <gmp.h>
#include <stdlib.h>

#include "randstate.h"

// initialize the external variable state
gmp_randstate_t state;

// Inializes the random state
// seed: the seed used to generate the random state
void randstate_init(uint64_t seed) {
    // use the Mersenne Twister algorithm to generate a random number
    gmp_randinit_mt(state);
    // use the seed to randomizie the state
    gmp_randseed_ui(state, seed);
}

// Clears the random state
void randstate_clear(void) {
    gmp_randclear(state);
}
