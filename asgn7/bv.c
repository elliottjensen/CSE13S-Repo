#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "bv.h"

// define the bit vector ADT as a series of uint8_ts
struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// Creates a bit vector
// Returns a bit vector of specified length
// length: specifies the length of the bit vector
BitVector *bv_create(uint32_t length) {
    // create and allocate space for the bit vector
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    bv->length = length;

    // make sure that the bit vector was created correctly before creating the actual
    // vector
    if (bv) {

        // if the length of the vector is not divisible by 8 we will need to allocate an
        // extra uint8_t for the excess bits
        int modulus = length % 8;
        int size = 0;
        if (modulus == 0) {
            size = length / 8;
        } else {
            size = (length / 8) + 1;
        }

        // create the array where the bits will be stored with the above calculated size
        bv->vector = (uint8_t *) calloc(size, sizeof(uint8_t));
        return bv;
    }
    return NULL;
}

// Deletes the bit vector
// bv: the bit vector to be deleted
void bv_delete(BitVector **bv) {
    // make sure that the array of bits exists before deleting it
    if ((*bv)->vector) {
        free((*bv)->vector);
    }

    // delete the bit vector
    if (*bv) {
        free(*bv);
    }
    // set the pointer to NULL
    *bv = NULL;
    return;
}

// Calculates the length of the bit vector
// Returns the length of the bit vector
// bv: the bit vector whose length will be returned
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// Sets the ith bit in a vector
// Returns false if the index is outside of the range
// Returns true if the setting of the bit was performed sucessfuly
// bv: the bit vector to be changed
// i: the index of the bit to be set
bool bv_set_bit(BitVector *bv, uint32_t i) {
    // if the index is outside of the range return false to indicate that the set
    // could not be performed
    if (i > bv->length) {
        return false;
    }
    bv->vector[i / 8] |= (1 << i % 8);
    return true;
}

// Clears the ith bit in the vector
// Returns false if the index is outside of the rangge
// Returns true if the clear was perforemd successfuly
// bv: the bitvector whose bit will be cleared
// i: the index of the bit to be cleared
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i > bv->length) {
        return false;
    }
    bv->vector[i / 8] &= ~(1 << i % 8);
    return true;
}

// Gets the bit at index i of bit vector bv
// Returns true (1) if the bit at index i is 1
// Returns false (0) if the bit at index i is 0
// bv: the bit vector for the bit to be gotten from
// i: the index of the bit to be checked
bool bv_get_bit(BitVector *bv, uint32_t i) {
    // if index is outside of range return false
    if (i > bv->length) {
        return false;
        // otherwise check if the bit at index i is 1
    } else if ((bv->vector[i / 8] >> i % 8) & 1) {
        // if so return true
        return true;
    } else {
        // otherwise return false
        return false;
    }
}

// Print the bits of a bit vector
// bv: the bit vector to be printed out
void bv_print(BitVector *bv) {
    // iterate over the bits of bv
    for (uint32_t i = 0; i < bv->length; i += 1) {
        // if the bit is a 1 print a 1
        if (bv_get_bit(bv, i)) {
            printf("1 ");
            // if the bit is a 0 print a 0
        } else {
            printf("0 ");
        }
    }
    printf("\n");
    return;
}
