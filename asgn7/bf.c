#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "salts.h"
#include "speck.h"
#include "bf.h"
#include "bv.h"

// define the bloom filter data tyepe
struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

// Initalizes the bloom filter where the badspeak and newspeak words will be stored
// size: the number of bits that should be allocated for the bloom filter
BloomFilter *bf_create(uint32_t size) {
    // allocate space for the bloom filter
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));

    // make sure that the bloom filter exists and that the allocation was performed
    // successfuly
    if (bf) {
        // create the bit vector where the filter will be stored
        bf->filter = bv_create(size);

        // initalize the salts for the three hash functions
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;
    }
    return bf;
}

// Deletes the bloom filter
// bf: the bloom filter to be deleted
void bf_delete(BloomFilter **bf) {
    // make sure that the bloom filter exists before trying to delete it
    if (*bf) {
        bv_delete(&(*bf)->filter); // delete the bit vector
        free(*bf); // free the pointer
        *bf = NULL; // set the pointer equal to NULL
    }
    return;
}

// Returns the size of the bloom filter
// bf: the bloom filter whose size will be returned
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// Inserts words into the bloom filter
// bf: the bloom filter where the word will be inserted into
// oldspeak: the word to be inserted
void bf_insert(BloomFilter *bf, char *oldspeak) {

    // hash oldspeak with each salt to calcualte each indece
    uint32_t i1 = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t i2 = hash(bf->secondary, oldspeak) % bf_size(bf);
    uint32_t i3 = hash(bf->tertiary, oldspeak) % bf_size(bf);

    bv_set_bit(bf->filter, i1);
    bv_set_bit(bf->filter, i2);
    bv_set_bit(bf->filter, i3);
    return;
}

// Probes the bloom filter for oldspeak
// Returns true if all of the bits of the indeces are set
// Returns false otherwise
// bf: the bloom filter to be probed
// oldspeak: the string to probe the bloom filter for
bool bf_probe(BloomFilter *bf, char *oldspeak) {

    // hash oldspeak with each salt to calculate each indece
    uint32_t i1 = hash(bf->primary, oldspeak) % bf_size(bf);
    uint32_t i2 = hash(bf->secondary, oldspeak) % bf_size(bf);
    uint32_t i3 = hash(bf->tertiary, oldspeak) % bf_size(bf);

    // store the get bit result in uint32_ts
    uint32_t probe1 = bv_get_bit(bf->filter, i1);
    uint32_t probe2 = bv_get_bit(bf->filter, i2);
    uint32_t probe3 = bv_get_bit(bf->filter, i3);

    // return true if all of the bits were 1
    if (probe1 && probe2 && probe3) {
        return true;
    }
    // otherwise return false
    return false;
}

// Calculates the number of set bits in the bloom filter
// Returns the number of set bits in the bloom filter
// bf: the bloom filter whose number of set bits will be returned
uint32_t bf_count(BloomFilter *bf) {
    uint32_t counter = 0; // initialize the count
    // iterate through the bloom filter
    for (uint32_t i = 0; i < bv_length(bf->filter); i += 1) {
        // only if the bit is a 1 do we increment count
        if (bv_get_bit(bf->filter, i)) {
            counter += 1;
        }
    }
    return counter;
}
// Prints the bloomfilter
// bf: the bloom filter to be printed
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
    return;
}
