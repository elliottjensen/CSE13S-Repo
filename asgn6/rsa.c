#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <gmp.h>
#include <stdbool.h>
#include <stdint.h>
#include "numtheory.h"
#include "randstate.h"

// Creates the compoments of the RSA public key
// p: the mpz where p, a large prime number, will be stored
// q: the variables where q, a large prime number, will be stored
// n: the public modulus mpz
// e: the public exponent mpz
// nbits: the size of p and q
// iters: the number of iterations for make_prime to run
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters) {
    // initialize values
    mpz_t pminus1, qminus1, phi, gcd_o;
    mpz_inits(pminus1, qminus1, phi, gcd_o, NULL);
    uint64_t b1, b2;

    // set the boundaries for p and q
    b1 = (random() % (nbits / 2)) + (nbits / 4);
    b2 = nbits - b1;

    // generate p and q
    make_prime(p, b1 + 1, iters);
    make_prime(q, b2 + 1, iters);

    // set n to p * q
    mpz_mul(n, p, q);

    // set p - 1 and q - 1
    mpz_sub_ui(pminus1, p, 1);
    mpz_sub_ui(qminus1, p, 1);

    // create the totient
    mpz_mul(phi, pminus1, qminus1);

    // generate values for e until e is coprime with the toient
    do {
        mpz_urandomb(e, state, nbits);
        gcd(gcd_o, e, phi);
    } while (mpz_cmp_ui(gcd_o, 1) != 0);

    /// clear the values
    mpz_clears(pminus1, qminus1, phi, gcd_o, NULL);
    return;
}

// Writes the public RSA key to the public file
// n: mpz public modulus n
// e: mpz public exponent e
// s: mpz siganture of the user
// username[]: character string username
// pbfile: the file where the rsa key will be written
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username);
    return;
}

// Reads the RSA key from the pbfile
// n: where the mpz public modulus will be stored
// e: where the mpz public exponent will be stored
// s: where the mpz user signature will be stored
// username[]: the character string where the username will be stored
// pbfile: where to read from
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n%Zx\n%Zx\n%s\n", n, e, s, username);
    return;
}

// Generates the private key d given primes p and q
// d: mpz where the private key will be stored
// e: mpz public exponent
// p: some large prime number composing the public key
// q: some large prime number composign the public key
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t temp1, temp2, temp3;
    mpz_inits(temp1, temp2, temp3, NULL);
    mpz_sub_ui(temp1, p, 1);
    mpz_sub_ui(temp2, q, 1);
    mpz_mul(temp3, temp1, temp2);
    mod_inverse(d, e, temp3);
    mpz_clears(temp1, temp2, temp3, NULL);
    return;
}

// Writes the RSA key to the private file
// n: the public modulus n, an mpz
// d: mpz private key
// pvfile: where to write the file
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", n, d);
    return;
}

// Reads the private RSA key from the private file
// n: an mpz for the public modulus n
// d: an mpz for the private key
// pvfile: where to read from
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", n, d);
    return;
}

// Performs RSA encryption
// c: mpz where the computed ciphertext is stored
// m: mpz where the original message is stored
// e: the public exponent
// n: the public modulus
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    pow_mod(c, m, e, n);
    return;
}

// Encrypts the contents of infile
// infile: the file to be encrypted
// outfile: where the ciphertext should be written
// n: the mpz with the public modulus
// e: the mzp with the public exponent
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    // set the size of how much fread is going to read at noe time
    uint64_t k = (mpz_sizeinbase(n, 2) - 1) / 8;

    // initialize the array
    uint8_t *array = (uint8_t *) calloc(k, sizeof(uint8_t));

    // initialize the output of fread
    int text = 1;

    // set the frist element of the array to FF
    array[0] = 0xFF;

    // intialzie variables
    mpz_t m, cipher_text;
    mpz_inits(m, cipher_text, NULL);

    // loop until you reach the end of the file
    while (!feof(infile)) {

        // read plain text from the innputfile
        text = fread(array + 1, sizeof(uint8_t), k - 1, infile);
        // store the text in m
        mpz_import(m, text + 1, 1, sizeof(uint8_t), 1, 0, array);

        // encypt m
        rsa_encrypt(cipher_text, m, e, n);

        // write the encrypted text to the output file
        gmp_fprintf(outfile, "%Zx\n", cipher_text);
    }
    // clear variables and free the array
    mpz_clears(m, cipher_text, NULL);
    free(array);
}

// Performs RSA decryption
// m: mpz where the decyrpted message will be stored
// c: mpz where the inputted ciphertext is located
// d: mpz d the private key
// n: mpz public modulus
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    pow_mod(m, c, d, n);
    return;
}

// Decrypts the contents of infile
// infile: mpz of ciphertext to be dectypted
// output: mpz where the plain text will be outputted
// n: mpz public modulus
// d: mpz private key
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    // initialize variables
    mpz_t m, cipher_text;
    mpz_inits(m, cipher_text, NULL);
    uint64_t k = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint64_t j = 1;
    uint8_t *array = (uint8_t *) calloc(k, sizeof(uint8_t));

    // set the virst varibale to FF
    array[0] = 0xFF;

    // read from infile until we reach the end of the file
    while (!feof(infile)) {

        // read ciphertext
        j = gmp_fscanf(infile, "%Zx\n", cipher_text);

        // decrypt cipher text and store the plain text in m
        rsa_decrypt(m, cipher_text, d, n);

        // export so that the plaintext into the array
        mpz_export(array, &j, 1, sizeof(uint8_t), 1, 0, m);

        // write from the array into the outfile
        fwrite((array + 1), sizeof(uint8_t), j - 1, outfile);
    }

    // clear variables and free the array
    mpz_clears(m, cipher_text, NULL);
    free(array);
    return;
}

// Perfroms RSA signing
// s: mpz for the signature to be stored in
// m: mpz for the username
// d: the private key
// n: the public modulus
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    pow_mod(s, m, d, n);
    return;
}

// Performs RSA verification
// m: the mpz converted username
// s: the signature of the user
// e: the public exponent e
// n: the publci modulus n
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    // initialize the temporary varibale t
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n);

    // if the username is equal to the power mod return true
    if (mpz_cmp(t, m) == 0) {
        mpz_clear(t);
        return true;
    }
    // otherwise return false
    mpz_clear(t);
    return false;
}
