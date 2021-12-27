#include "mathlib.h"

#include <stdio.h>

static int terms_e;

// Calculates e using Taylor Series
// Returns an approximation of e
double e(void) {
    double z; // previous term
    double y; // current term
    double to_add; // amount to increase y by
    int k; // variable k in the summation; increases by 1 each iteration
    double factorial; //the denominator factorial
    z = 0;
    y = 0;
    k = 0;
    factorial = 1;

    do {
        terms_e += 1; //increment terms
        z = y; // current term becomoes previous term
        to_add = (1 / factorial); // calculates amount to add
        y += to_add; // y becomes closer to e
        k += 1; // prepare k for next iteration of the sum
        factorial *= k; // update factorial

        if (k > 100) { // prevents endless loops
            printf("something went wrong; over 100 terms");
            break;
        }
    } while (absolute(y - z) > EPSILON); // End once the amount changed cannot be represented
    return y;
}

// Calculates the number of terms used by e()
// returns intiger number of terms
int e_terms(void) {
    return terms_e;
}
