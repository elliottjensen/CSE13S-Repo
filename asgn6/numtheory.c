#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <gmp.h>

#include "numtheory.h"
#include "randstate.h"

// Finds the gcd d of a and b
// a: some mpz_t
// b: some mpz_t
// d: some mpz_t where the gcd will be stored
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    // initialziing variables
    mpz_t t, a1, b1;
    mpz_inits(t, a1, b1, NULL);

    //creating temporary variables so that the inputs are unchanged
    mpz_set(a1, a);
    mpz_set(b1, b);

    // loop until b = 0
    while (mpz_cmp_ui(b1, 0) != 0) {
        mpz_set(t, b1);
        mpz_mod(b1, a1, b1);
        mpz_set(a1, t);
    }

    //set d to gcd of a and b
    mpz_set(d, a1);
    mpz_clears(a1, b1, t, NULL);
}

// Computes (base ^ exponent) % modulus
// base: the base mpz_t
// exponent: the exponent mpz_t
// modulus: the modulus mpz_t
void pow_mod(mpz_t out, mpz_t base, mpz_t exponent, mpz_t n) {
    // initialize the values
    mpz_t v;
    mpz_init(v);
    mpz_t p;
    mpz_init(p);

    // duplicate the exponent so that the inputs are unchanged
    mpz_t exp1;
    mpz_init(exp1);
    mpz_set(exp1, exponent);

    // set v to 1 and p to base
    mpz_set_ui(v, 1);
    mpz_set(p, base);

    // loop until
    while (mpz_cmp_ui(exp1, 0) > 0) {
        if (mpz_odd_p(exp1) != 0) {
            mpz_mul(v, v, p);
            mpz_mod(v, v, n);
        }

        mpz_mul(p, p, p);
        mpz_mod(p, p, n);

        mpz_fdiv_q_ui(exp1, exp1, 2);
    }
    mpz_set(out, v);
    mpz_clears(v, p, exp1, NULL);
    return;
}

// Computes the inverse i of a modulo n
// i: an mpz where the output will be stored
// a: the mpz for the lhs of the modulus
// n: the mpz for the rhs of the modulus
void mod_inverse(mpz_t i, mpz_t a, mpz_t n) {
    // initializing variables
    mpz_t q;
    mpz_init(q);

    mpz_t r;
    mpz_init(r);

    mpz_t r_prime;
    mpz_init(r_prime);

    mpz_t t;
    mpz_init(t);

    mpz_t t_prime;
    mpz_init(t_prime);

    // r = n and r_prime = a
    mpz_set(r, n);
    mpz_set(r_prime, a);

    // t = 0 and t_prime = 1
    mpz_set_ui(t, 0);
    mpz_set_ui(t_prime, 1);

    // initialize temporary variables
    mpz_t output;
    mpz_init(output);

    mpz_t arg1;
    mpz_t arg2;

    mpz_init(arg1);
    mpz_init(arg2);

    // loop until r_prime is equal to 0
    while (mpz_cmp_ui(r_prime, 0) != 0) {

        // set q to r divided by r_prime
        mpz_fdiv_q(q, r, r_prime);

        // create duplicates of r and r_prime
        mpz_set(arg1, r);
        mpz_set(arg2, r_prime);

        // compute: (r,r') <- (r', r-q*r')
        mpz_set(r, r_prime);
        mpz_mul(output, q, arg2);
        mpz_sub(r_prime, arg1, output);

        // create duplicates of t and t_prime
        mpz_set(arg1, t);
        mpz_set(arg2, t_prime);

        // compute: (t,t') <- (t', t-q*t')
        mpz_set(t, t_prime);
        mpz_mul(output, q, arg2);
        mpz_sub(t_prime, arg1, output);
    }
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(i, 0);
    } else if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
        mpz_set(i, t);
    } else {
        mpz_set(i, t);
    }
    mpz_clears(q, r, r_prime, t, t_prime, arg1, arg2, output, NULL);
    return;
}

// Conducts Miller-Rabin primality test
bool is_prime(mpz_t n, uint64_t iters) {
    // initialize and set values
    mpz_t r, nminus1, a, y, j, two, bounds;
    mpz_inits(r, nminus1, a, y, j, two, bounds, NULL);
    mp_bitcnt_t s = 0;
    mpz_set_ui(r, 0);
    mpz_set_ui(two, 2);
    mpz_set_ui(a, 0);

    mpz_sub_ui(nminus1, n, 1);

    // take care of base cases
    if (mpz_cmp_ui(n, 0) == 0) {
        mpz_clears(r, nminus1, a, y, j, two, bounds, NULL);
        return false;
    } else if (mpz_cmp_ui(n, 1) == 0) {
        mpz_clears(r, nminus1, a, y, j, two, bounds, NULL);
        return false;
    } else if (mpz_cmp_ui(n, 2) == 0) {

        mpz_clears(r, nminus1, a, y, j, two, bounds, NULL);
        return true;
    } else if (mpz_cmp_ui(n, 3) == 0) {

        mpz_clears(r, nminus1, a, y, j, two, bounds, NULL);
        return true;
    } else if (mpz_cmp_ui(n, 4) == 0) {

        mpz_clears(r, nminus1, a, y, j, two, bounds, NULL);
        return false;
    }

    // performs a right logical shift on r until it is odd
    while (mpz_even_p(r)) {
        mpz_tdiv_q_2exp(r, nminus1, s);
        mpz_fdiv_q_ui(r, r, 2);
        s += 1;
    }

    mp_bitcnt_t sminus1 = s - 1;
    mpz_sub_ui(bounds, n, 3);

    // taken from assignment doc sudo code
    for (uint64_t i = 1; i < iters; i += 1) {

        // generate a random witness
        mpz_urandomm(a, state, bounds);

        // shift the range of the random number up 1
        mpz_add_ui(a, a, 2);
        // set y to (a ^ r) % n
        pow_mod(y, a, r, n);
        if ((mpz_cmp_ui(y, 1) != 0) && (mpz_cmp(y, nminus1) != 0)) {
            mpz_set_ui(j, 1);
            while ((mpz_cmp_ui(j, sminus1) <= 0) && (mpz_cmp(y, nminus1) != 0)) {
                pow_mod(y, y, two, n);
                if (mpz_cmp_ui(y, 1) == 0) {

                    // clear variables before returning
                    mpz_clears(r, nminus1, a, y, j, two, bounds, NULL);
                    return false;
                }

                // increment j
                mpz_add_ui(j, j, 1);
            }
            if (mpz_cmp(y, nminus1) != 0) {

                //clear variables before returning
                mpz_clears(r, nminus1, a, y, j, two, bounds, NULL);
                return false;
            }
        }
    }

    // clear variables before returning
    mpz_clears(r, nminus1, a, y, j, two, bounds, NULL);

    // if no factors were found return true
    return true;
}

// Generates a prime number
// p: mpz where the prime number will be stored
// bits: the minimum size of the prime number
// iters: the number of iterations that increases the level of certainty of the prime
// number
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {

    // initialize variables
    mpz_t random_num, math, range, one;
    mpz_inits(random_num, math, range, one, NULL);

    // mpz_urandomb sets the upper bound exclusively, so we increase by 1 so that bits
    // is the maximum not bits - 1
    mpz_set_ui(range, bits);
    mpz_add_ui(range, range, 1);

    // mpz_mul_2exp's second argument accepts an mpz, and multiplies it by 2 and uses
    // it as the base for 2 ^ (n - 1), so we need to create a mpz with the value 1
    mpz_set_ui(one, 1);

    //this acts as the n - 1 of 2 ^(n - 1)
    mp_bitcnt_t bit_count = bits - 1;

    // create a number equal to 2 ^ (n - 1)
    // this number will be bits long with a 1 in the first position and 0s
    // everywhere else
    mpz_mul_2exp(math, one, bit_count);

    // generate random numbers at least nbits in size until we find one that is coprime
    // to the totient: phi
    do {
        // creates a random number with bits as a maximum
        mpz_urandomb(random_num, state, bits);
        //by adding 2 ^ (n - 1) we are able to make bits the minimum
        mpz_add(random_num, random_num, math);
    } while (!is_prime(random_num, iters));

    // once a suitable prime is found set p to it
    mpz_set(p, random_num);

    // clear variables
    mpz_clears(random_num, math, range, one, NULL);
    return;
}
