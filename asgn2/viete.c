#include "mathlib.h"

#include <stdio.h>

static int viete_factors = 0;

// Calculates pi using Viete's formula
// Returns an approximation of pi in a double
double pi_viete(void) {
    double z; // previous term
    double y; // current term
    double to_mult; // amount to multiply y by

    double k; // keeps track of which term the function is on

    z = 1;
    y = 1;
    k = 1;
    to_mult = sqrt_newton(2);

    do {
        viete_factors += 1; // increases number of terms for pi_viete_factors() function

        z = y; // current term becomes the previous term
        y *= ((to_mult) / 2); // calculates how much to multiply y by
        to_mult = sqrt_newton(2 + to_mult); // updates to_mult for next iteration
        k += 1; // moves k to next term

        if (k > 1000) { // prevents infinite loops
            break;
        }
    } while (absolute(y - z) > EPSILON); // ends loop once change beccomes too small to represent
    return 2 / y; //performs final caculation
}

// Calculates number of terms used by pi_viete()
// Returns integer number of factors
int pi_viete_factors() {
    return viete_factors;
}
