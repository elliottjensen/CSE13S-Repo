#include "mathlib.h"

#include <stdio.h>

static int bbp_terms = 0;

// Calculates pi using the Bailey-Borwein-Plouffe Formula
// Returns an approximation of pi
double pi_bbp(void) {

    double z; // previous approximation
    double y; // current approximation
    double to_add; // amount added each time
    int k; // counts iterations
    double exterior; // the part of the equation outside of the parentheses
    z = 0;
    y = 0;
    k = 0;
    exterior = 1;

    do {
        bbp_terms += 1; //increments number of terms
        z = y; // previous  becomes current
        to_add = exterior * ((120 * k + 151) * k + 47)
                 / (k * (k * (k * (512 * k + 1024) + 712) + 194)
                     + 15); // plug in variables into equation

        y += to_add; // y becomes more accurate
        k += 1; //increment k in the equation
        exterior /= 16; // exterior becomes smaller each loop
        if (k > 100) { // breaks infinite loops
            break;
        }
    } while (absolute(y - z)
             > EPSILON); //ends once the amount changed becomes too small to be represented
    return y;
}

// returns the number of terms used to calculte pi in pi_bbp()
int pi_bbp_terms(void) {
    return bbp_terms;
}
