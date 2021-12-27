#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <regex.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "bv.h"
#include "bf.h"
#include "ht.h"
#include "parser.h"
#include "messages.h"

// define the regex word
#define WORD "[a-zA-Z0-9_'-]+"

int main(int argc, char **argv) {

    // initialize command line booleans
    bool print_help = false;
    bool print_stats = false;

    // set default ht and bf size
    uint32_t ht_size1 = 65636;
    uint32_t bf_size1 = 1048576;

    // open up badseak and newspeak files
    FILE *badspeak = fopen("badspeak.txt", "r");
    FILE *newspeak = fopen("newspeak.txt", "r");

    // initialize booleans for which message to print out
    bool thought_crime = false;
    bool counseling = false;

    int opt = 0;

    // parse through commadn line options
    while ((opt = getopt(argc, argv, "hmst:f:")) != -1) {
        switch (opt) {
        case 'h': print_help = true; break;
        case 's': print_stats = true; break;
        case 't': ht_size1 = atoi(optarg); break;
        case 'f': bf_size1 = atoi(optarg); break;
        default: print_help = true; break;
        }
    }

    if (print_help) {
        printf("OPTIONS\n"
               "   -h          Program usage and help.\n"
               "   -s          Print program statistics.\n"
               "   -t size     Specify hash table size (default: 2^16).\n"
               "   -f size     Specify Bloom filter size (default: 2^20).\n");
        return -1;
    }

    // initialize the bf, ht and buffers
    BloomFilter *bf = bf_create(bf_size1);
    HashTable *ht = ht_create(ht_size1);
    char buf[4096];
    char buf1[4096];

    // add the badspeak words to the bloom filter and hash table
    while (fscanf(badspeak, "%s", buf) != EOF) {
        bf_insert(bf, buf);
        ht_insert(ht, buf, NULL);
    }
    fclose(badspeak);

    // add the newspeak word pairs to the bloom filter and the hash table
    while ((fscanf(newspeak, "%s %s", buf, buf1)) != EOF) {
        bf_insert(bf, buf);
        ht_insert(ht, buf, buf1);
    }
    fclose(newspeak);

    // initialize the binary search trees where the bad words in the input text will be
    // stored
    Node *old = bst_create();
    Node *oldnew = bst_create();

    // compile regex and return an error if it cannot be compiled
    regex_t re;
    if (regcomp(&re, WORD, REG_EXTENDED)) {
        fprintf(stderr, "Failed to compile regex.\n");
        return 1;
    }

    Node *node;
    char *word = NULL;

    // iterate through the input text
    while ((word = next_word(stdin, &re)) != NULL) {

        // convert all words to their lower case versions
        for (uint32_t i = 0; i < strlen(word); i += 1) {
            word[i] = tolower(word[i]);
        }

        // check if the word is in the bloom filter
        if (bf_probe(bf, word)) {
            // if it is in the bloom filter check if it is in the hahs table
            node = ht_lookup(ht, word);
            if (node == NULL) {
                // if it was not in the hash table move on to the next word
                continue;
            }
            // if it is a word that has a translation then the citizen needs counseling
            if (node->newspeak && node->oldspeak) {
                counseling = true;
                // add the word they used the binary search tree
                oldnew = bst_insert(oldnew, node->oldspeak, node->newspeak);
                // if it is a word without a translation the citizen has commited thought
                // crime
            } else if (node->oldspeak) {
                thought_crime = true;
                // add the badspeak word to the binary search tree
                old = bst_insert(old, node->oldspeak, NULL);
            }
        }
    }

    // if the command line option '-s' was specified suppress the message print and
    // only print the statistics of the program
    if (print_stats) {

        // peform the calculations of the statistics
        double branches_traversed = ((double) branches / lookups);
        double ht_load = 100 * ((double) ht_count(ht) / ht_size(ht));
        double bf_load = 100 * ((double) bf_count(bf) / bf_size(bf));

        // print the statistics out

        printf("Average BST size: %.6f\n", ht_avg_bst_size(ht));
        printf("Average BST height: %.6f\n", ht_avg_bst_height(ht));
        printf("Average branches traversed: %.6f\n", branches_traversed);
        printf("Hash table load: %.6f%%\n", ht_load);
        printf("Bloom filter load: %.6f%%\n", bf_load);
        return 0;
    }

    // if statistics were not specified in thte command line print out the appropraite
    // message
    // if thoughtcrime was commited and counseling is required print out the mixspeak
    // message
    if (thought_crime && counseling) {
        //printf("thought_crime ^ counseling\n");
        printf("%s", mixspeak_message);
        bst_print(old);
        bst_print(oldnew);
        // if only thought crime was commited print out the baspeak message
    } else if (thought_crime) {
        //printf("thought_crime\n");
        printf("%s", badspeak_message);
        bst_print(old);
        // if only counseling is required print out the goodspeak message
    } else if (counseling) {
        //printf("counseling\n");
        printf("%s", goodspeak_message);
        bst_print(oldnew);
    }

    // clear all ADTs
    clear_words();
    regfree(&re);
    bf_delete(&bf);
    ht_delete(&ht);
    bst_delete(&old);
    bst_delete(&oldnew);
    return 0;
}
