#include <stdio.h>
#include <stdlib.h>

#include "speck.h"
#include "bst.h"
#include "ht.h"
#include "salts.h"

// initialize the global variable lookups for the sake of statistics
uint64_t lookups = 0;

// define the ADT HashTable with a 128 bit salt and dfine a root node
struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

// Create the hash table
// size: defines the size of the hash table
HashTable *ht_create(uint32_t size) {
    // initialize and allocate space for the hash table
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));

    // make sure that the ht exists
    if (ht) {

        // initialize the salts
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;

        ht->size = size; // set the size

        // create the actual hashtable of trees
        ht->trees = (Node **) calloc(size, sizeof(Node *));
    }
    return ht;
}

// Deletes the hashtable
// ht: the hashtable to be deleted
void ht_delete(HashTable **ht) {
    // iterate through the series of trees in the hash table and delete each one
    for (uint32_t i = 0; i < (*ht)->size; i += 1) {
        if ((*ht)->trees[i]) {
            bst_delete(&(*ht)->trees[i]);
        }
    }
    // delete the hash table and set the pointer to NULL
    free((*ht)->trees);
    free(*ht);
    *ht = NULL;
    return;
}

// Returns the size of the hash table
// ht: the hashtable whose size will be returned
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// Determines if a string exists in the hashtable exists. If it does it returns the node
// with that word. Otherwise it returns NULL
// ht: the hashtable to be searched
// oldspeak: the string to searh the hashtable for
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups += 1; // increment lookups for the sake of statistics
    uint32_t index = hash(ht->salt, oldspeak) % ht->size; // find the index of the bst
    Node *n = bst_find(ht->trees[index], oldspeak); // look in the bst for the string
    return n;
}

// Inserts a node into a hashtable
// ht: the hashtable the node will be inserted into
// oldspeak: the first string that will be stored in the node
// newspeak the second string that will be stored in the node
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups += 1; // increment lookups for the sake of statistics
    uint32_t index = hash(ht->salt, oldspeak) % ht->size; // find the index of the bst

    // replace the tree with a new tree with the node inserted
    ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
    return;
}

// Calculates the number of nodes in the hash table
// ht: the hashtable whose count will be calculated
uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0; // initialize count
    for (uint32_t i = 0; i < ht->size; i += 1) { // iterate through each member of the ht
        if (ht->trees[i] != NULL) { // makesure that the ht is not empty
            count += 1;
        }
    }
    return count;
}

// Calculates the average size of the bsts within the ht
// ht: the hashtable whose average bst size will be calculated
double ht_avg_bst_size(HashTable *ht) {
    // initialize the numerator and denominator
    double final_sum = 0;
    double size1 = ht_count(ht);

    // iterate through the hashtable incrementing the sum by the size of each bst
    for (uint32_t i = 0; i < ht->size; i += 1) {
        final_sum += bst_size(ht->trees[i]);
    }

    // return the sum divided by the size
    return final_sum / size1;
}

// Calculates the average height of the bsts within the hashtable
// ht: the hashtable whose average bst height will be calculated
double ht_avg_bst_height(HashTable *ht) {
    // initialize the numerator and denominator
    double final_sum = 0;
    double size1 = ht_count(ht);

    // iterate through the size of the ht and increment the sum by the height of each
    // bst within the hashtable
    for (uint32_t i = 0; i < ht->size; i += 1) {
        final_sum += bst_height(ht->trees[i]);
    }
    // return the summed heights divided by the number of bsts
    return final_sum / size1;
}

//Prints the hashtable
// ht: the hashtable to be printed out
void ht_print(HashTable *ht) {
    printf("%d\n", ht->size);
}
