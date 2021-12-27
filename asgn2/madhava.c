#include "mathlib.h"

#include <stdio.h>

static int madhava_terms = 0;

// Calculates pi using Madhava Formula
// Returns a double approximation of pi
double pi_madhava() {
    double z; // previous term
    double y; // current term
    double to_add; // amount to increase y by
    int k; // keeps track of which term we are on
    double numerator; // keeps track fo the numerator value separately
    z = 0;
    y = 0;
    k = 0;
    numerator = 1;
    do {
        madhava_terms += 1; //incremented for term function
        z = y; // current term becomes the previous term
        to_add = (numerator / ((2 * k) + 1)); // calculates amount to move y by

        // if we are on an even term (k) add, otherwise subtract
        if (k % 2 == 0) {
            y += to_add;
        } else {
            y -= to_add;
        }

        k += 1; // move on to next term
        numerator /= 3; // reduces numerator for next term

        if (k > 100) { // prevents infinite loops
            break;
        }
    } while (absolute(y - z)
             > EPSILON); // end calculation once difference becomes too small to represent
    return y * sqrt_newton(12); // perform final calculation
}
// Calculates number of terms used by pi_madhava()
// Returns integer terms
int pi_madhava_terms(void) {
    return madhava_terms;
}
