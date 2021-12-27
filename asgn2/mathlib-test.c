#include "mathlib.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "aebmrvnsh"

int main(int argc, char **argv) {
    int opt = 0;

    bool test_bbp = false;
    bool test_e = false;
    bool test_euler = false;
    bool test_madhava = false;
    bool test_newton = false;
    bool test_viete = false;

    bool enable_statistics = false;
    bool display_help = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            test_bbp = true;
            test_e = true;
            test_euler = true;
            test_madhava = true;
            test_newton = true;
            test_viete = true;
            break;
        case 'e': test_e = true; break;
        case 'b': test_bbp = true; break;
        case 'm': test_madhava = true; break;
        case 'r': test_euler = true; break;
        case 'v': test_viete = true; break;
        case 'n': test_newton = true; break;
        case 's': enable_statistics = true; break;
        case 'h': display_help = true; break;
        }
    }

    double i;

    if (test_bbp) {
        printf("pi_bbp() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_bbp(), M_PI,
            absolute(M_PI - pi_bbp()));

        if (enable_statistics) {
            printf("pi_bbp() terms = %d\n", pi_bbp_terms());
        }
    }

    if (test_e) {
        //printf("aSsErTiOn: %16.15f\n", absolute(2.718281828452354 - e()));
        printf("e() = %16.15lf, M_E = %16.15lf, diff = %16.15lf\n", e(), M_E, absolute(M_E - e()));

        if (enable_statistics) {
            printf("e terms = %d\n", e_terms());
        }
    }

    if (test_euler) {
        printf("pi_euler() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_euler(), M_PI,
            absolute(M_PI - pi_euler()));

        if (enable_statistics) {
            printf("pi_euler terms = %d\n", pi_euler_terms());
        }
    }

    if (test_madhava) {
        printf("pi_madhava() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_madhava(), M_PI,
            absolute(M_PI - pi_madhava()));

        if (enable_statistics) {
            printf("pi_madhava terms = %d\n", pi_madhava_terms());
        }
    }

    if (test_viete) {
        printf("pi_viete() = %16.15lf, M_PI = %16.15lf, diff = %16.15lf\n", pi_viete(), M_PI,
            absolute(M_PI - pi_viete()));
        //printf("aSsErTiOn %16.15f\n", absolute(pi_viete() - 2.1415926535897932846));
        if (enable_statistics) {
            printf("pi_viete factors = %d\n", pi_viete_factors());
        }
    }

    if (test_newton) {
        for (i = 0; i < 10; i = i + 0.1) {
            printf("sqrt_newton(%f) = %16.15lf, sqrt(%f) = %15.14lf, diff = %16.15lf\n", i,
                sqrt_newton(i), i, sqrt(i), absolute(sqrt_newton(i) - sqrt(i)));

            if (enable_statistics) {
                printf("sqrt_newton iters %d\n", sqrt_newton_iters());
            }
        }
    }
    return 0;
}
