#include "mathlib.h"

#include <stdio.h>

static int newton_iters = 0;

// Calculates square root using the Newton-Raphson method
// Returns approximate square root of double inputted
//
// x: any positive double or 0
double sqrt_newton(double x) {
    newton_iters = 0; //counts iteration for sqrt_newton_iters()
    double z; // previous term
    double y; // current term
    z = 0;
    y = 1;
    while (absolute(y - z) > EPSILON) { // stops once amount changed becomes too small to represent
        newton_iters += 1;
        z = y; // previous term becomes current term
        y = 0.5 * (z + x / z); // performs calculation
    }
    return y;
}

// Calculates number of terms used by sqrt_newton()
// Returns integer number of terms
int sqrt_newton_iters(void) {
    return newton_iters;
}
