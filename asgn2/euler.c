#include "mathlib.h"

#include <stdio.h>

static int euler_terms = 0;

// calculates pi using Euler's formula
// returns an approximation of pi
double pi_euler(void) {
    double z; // previous term
    double y; // current term
    double to_add; // amount to increase y by
    double k; // keeps track of which term is currently being done

    z = 0;
    y = 0;
    k = 1;

    do {
        euler_terms += 1; //counts terms for term function
        z = y; // current term becomes previous term
        to_add = (1 / (k * k)); // calculates amount to increase y by

        y += to_add; // y becomes closer to pi

        k += 1; // move on to next term

        if (k > 100000000) { // prevents infinite loops
            break;
        }
    } while (
        absolute(y - z) > EPSILON); // end calculation once change becomes too small to represent
    return sqrt_newton(y * 6); //performs last operation in formula
}

// Calculates number of terms used by pi_euler
// Returns integer number of terms
int pi_euler_terms(void) {
    return euler_terms;
}
