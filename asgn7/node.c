#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "node.h"

// Creates a node
// oldspeak: the string that will be stored in the node
// newspeak: the corresponding replacement word. set this to null if there is no
// replacement word
Node *node_create(char *oldspeak, char *newspeak) {
    // create the node and allocte space for it
    Node *n = (Node *) malloc(sizeof(Node));
    // if there is no node return NULL
    if (n == NULL) {
        return NULL;
    }

    // initialize left and right by setting them to NULL first
    n->left = NULL;
    n->right = NULL;

    // if oldspeak or oldspeak and newspeak were passed in as arguments set the node
    // to have those strings
    if (oldspeak) {
        n->oldspeak = strdup(oldspeak);
    } else {
        n->oldspeak = NULL;
    }
    if (newspeak) {
        n->newspeak = strdup(newspeak);
    } else {
        n->newspeak = NULL;
    }
    return n;
}

// Deletes a node
// n: the node to be deleted
void node_delete(Node **n) {

    // make sure the node and the oldspeak and newspeak exist before deleting them
    if (*n) {
        if ((*n)->oldspeak) {
            free((*n)->oldspeak);
        }
        if ((*n)->newspeak) {
            free((*n)->newspeak);
        }
        free(*n);
        *n = NULL;
    }
    return;
}

// Prints a node
// n: the node to be printed
void node_print(Node *n) {
    // make sure the node exists before printing it
    if (n) {
        // if the node contains an oldspeak and newspeak word print them both and
        // separate them with an arrow
        if (n->oldspeak && n->newspeak) {
            printf("%s -> %s\n", n->oldspeak, n->newspeak);
            // otherwise just print out the oldspeak word
        } else {
            printf("%s\n", n->oldspeak);
        }
    }
    return;
}
