#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "rsa.h"

int main(int argc, char **argv) {
    // initialize values
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *keyfile;

    bool print_help = false;
    bool verbose_printing = false;
    bool key_default = true;

    int opt = 0;
    // parse through command line options
    while ((opt = getopt(argc, argv, "i:o:n:vh")) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n':
            keyfile = fopen(optarg, "r");
            if (keyfile == NULL) {
                fprintf(stderr, "Error: failed to open keyfile\n");
            }
            key_default = false;
            break;
        case 'v': verbose_printing = true; break;
        case 'h': print_help = true; break;
        default: print_help = true; return -1;
        }
    }

    // if no keyfile is specified use rsa.priv
    if (key_default) {
        keyfile = fopen("rsa.priv", "r");
    }
    // by default and if specified by the user print out help message
    if (print_help) {
        printf("SYNOPSIS\n"
               "   Decrypts file\n"
               "USAGE\n"
               "   ./decrypt [-hv] -i infile -o outfile -n pvfile\n"
               "   -h      Display program help and usage.\n"
               "   -v      Display verbose program output.\n"
               "   -i      Input file (default: stdin).\n"
               "   -o      Output file (deault: stdout).\n"
               "   -n      Key file (default: rsa.pub).\n");
    }

    // initialize mpzs
    mpz_t n, d;
    mpz_inits(n, d, NULL);

    // read from the private key
    rsa_read_priv(n, d, keyfile);

    // read ciphertext from infile and write plaintext to the outfile
    rsa_decrypt_file(infile, outfile, n, d);

    // if the user specifies verbose printing print n and d and their number of bits
    if (verbose_printing) {
        int n_size = mpz_sizeinbase(n, 2);
        int d_size = mpz_sizeinbase(d, 2);

        gmp_printf("n (%d bits): %Zd\n", n_size, n);
        gmp_printf("d (%d bits): %Zd\n", d_size, d);
    }

    // close files and clear variables
    fclose(infile);
    fclose(outfile);
    fclose(keyfile);
    mpz_clears(n, d, NULL);
    return 0;
}
