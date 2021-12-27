#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <gmp.h>
#include <stdint.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "randstate.h"
#include "numtheory.h"
#include "rsa.h"

int main(int argc, char **argv) {
    // initialize variables
    uint64_t seed = time(NULL);
    FILE *pbfile;
    FILE *pvfile;

    uint64_t iters = 50;
    uint64_t nbits = 256;

    bool verbose_printing = false;
    bool pb_default = true;
    bool pv_default = true;
    int opt = 0;
    bool print_help = false;

    // parse through command line options
    while ((opt = getopt(argc, argv, "b:i:n:d:s:vh")) != -1) {
        switch (opt) {
        case 'b': nbits = (int) optarg; break;
        case 'i': iters = (int) optarg; break;

        case 'n':
            //printf("reading from specified pbfile\n");
            pbfile = fopen(optarg, "w+");

            if (pbfile == NULL) {
                fprintf(stderr, "Error: failed to open pbfile\n");
            }
            pb_default = false;
            break;
        case 'd':
            //printf("reading from specified pvfile\n");
            pvfile = fopen(optarg, "w+");
            if (pvfile == NULL) {
                fprintf(stderr, "Error: failed to open pvfile\n");
            }
            pv_default = false;
            break;
        case 's':
            //printf("optarg: %s\n", optarg);
            seed = atoi(optarg);
            //printf("seed %lu\n", seed);

            break;
        case 'v': verbose_printing = true; break;
        case 'h': print_help = true; break;
        default: print_help = true; return -1;
        }
    }
    // if no public file was specified set it to rsa.pub
    if (pb_default) {
        pbfile = fopen("rsa.pub", "w+");
    }
    // if no private file was speciefied set it to rsa.priv
    if (pv_default) {
        pvfile = fopen("rsa.priv", "w+");
    }

    if (print_help) {
        printf("SYNOPSIS\n"
               "    Generates an RSA public/private key pair.\n"
               "USAGE\n"
               "    ./keygen [-hv] [-b bits] -n pbfile -d pvfile\n"
               "OPTIONS\n"
               "    -h              Display program help and usage.\n"
               "    -v              Display verbose program output.\n"
               "    -b bits         Minimum bits needed for public key n.\n"
               "    -c confidence   Miller-Rabin iterations for testing primes (default 50).\n"
               "    -n pbfile       Public key file (default: rsa.pub).\n"
               "    -d pvfile       Private key file (default: rsa.priv).\n"
               "    -s seed         Random seed for testing.\n");
    }

    // initialize the seed and variables
    randstate_init(seed);
    mpz_t p, q, n, e, d;
    mpz_inits(d, p, q, n, e, NULL);

    // make the public and private keys
    rsa_make_pub(p, q, n, e, nbits, iters);
    rsa_make_priv(d, e, p, q);

    // store the username as a sting
    char *username = getenv("USER");

    // convert username into a mpz
    mpz_t mpz_username;
    mpz_init(mpz_username);
    mpz_set_str(mpz_username, username, 62);

    mpz_t s;
    mpz_init(s);

    // create signature
    rsa_sign(s, mpz_username, d, n);

    // write the public and private keys
    rsa_write_pub(n, e, s, username, pbfile);
    rsa_write_priv(n, d, pvfile);

    if (verbose_printing) {
        int s_size = mpz_sizeinbase(s, 2);
        int p_size = mpz_sizeinbase(p, 2);
        int q_size = mpz_sizeinbase(q, 2);
        int n_size = mpz_sizeinbase(n, 2);
        int e_size = mpz_sizeinbase(e, 2);
        int d_size = mpz_sizeinbase(d, 2);

        gmp_printf("%s\n", username);
        gmp_printf("s (%d bits): %Zd\n", s_size, s);
        gmp_printf("p (%d bits): %Zd\n", p_size, p);
        gmp_printf("q (%d bits): %Zd\n", q_size, q);
        gmp_printf("n (%d bits): %Zd\n", n_size, n);
        gmp_printf("e (%d bits): %Zd\n", e_size, e);
        gmp_printf("d (%d bits): %Zd\n", d_size, d);
    }

    fclose(pbfile);
    fclose(pvfile);

    // free memory
    mpz_clears(mpz_username, p, q, n, e, s, d, NULL);
    randstate_clear();
    return 0;
}
