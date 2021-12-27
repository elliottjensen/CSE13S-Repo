#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include "rsa.h"

int main(int argc, char **argv) {
    // initialize variables
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *keyfile;

    bool print_help = false;
    bool verbose_printing = false;
    bool key_default = true;

    int opt = 0;

    // parse throug command line optoins
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

    // if a keyfiel was not specified  set the defualt to rsa.pub
    if (key_default) {
        keyfile = fopen("rsa.pub", "r");
    }
    // if asked and by default print help message
    if (print_help) {
        printf("SYNOPSIS\n"
               "   Encypts file\n"
               "USAGE\n"
               "   ./encypt [-hv] -i infile -o outfile -n pbfile\n"
               "   -h      Display program help and usage.\n"
               "   -v      Display verbose program output.\n"
               "   -i      Input file (default: stdin).\n"
               "   -o      Output file (deault: stdout).\n"
               "   -n      Key file (default: rsa.pub).\n");
    }

    // initialize values
    mpz_t n, e, s, m;
    mpz_inits(n, e, s, m, NULL);
    char username[128];

    // read from the public key
    rsa_read_pub(n, e, s, username, keyfile);

    //create the username string
    mpz_set_str(m, username, 62);

    // verify the user
    if (!rsa_verify(m, s, e, n)) {
        fprintf(stderr, "Invalid signature\n");
        return -1;
    }

    // encrypt the file
    rsa_encrypt_file(infile, outfile, n, e);

    // if verbose_printing is specified print n and e
    if (verbose_printing) {
        int n_size = mpz_sizeinbase(n, 2);
        int e_size = mpz_sizeinbase(e, 2);

        gmp_printf("n (%d bits): %Zd\n", n_size, n);
        gmp_printf("e (%d bits): %Zd\n", e_size, e);
    }

    // clear variables and close files
    mpz_clears(n, e, s, m, NULL);
    fclose(infile);
    fclose(outfile);
    fclose(keyfile);
    return 0;
}
