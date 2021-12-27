#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>

#include "node.h"

Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->symbol = symbol;
        n->frequency = frequency;
        //printf("n->symbol %d\n", n->symbol);
        //printf("n->frequency %lu\n", n->frequency);
        //n->left = NULL;
        //n->right = NULL;
    }
    return n;
}

void node_delete(Node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
    return;
}

Node *node_join(Node *left, Node *right) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->frequency = left->frequency + right->frequency;
        n->symbol = '$';
        n->right = right;
        n->left = left;
    }
    return n;
}

void node_print(Node *n) {
    printf("(%d,%lu) ", n->symbol, n->frequency);

    if (n->left) {
        printf("left node:\n");
        node_print(n->left);
    }
    if (n->right) {
        printf("right node:\n");
        node_print(n->right);
    }
    printf("going back\n");
    /*
   //printf("running node print\n");
    if (n) {
        printf("parent:");
        printf("(%d,%lu) ", n->symbol, n->frequency);
        printf("left:");
        node_print(n->left);
	printf("right:");
        node_print(n->right);
	printf("\n");
    }
    */
    return;
}
