#include <stdio.h>

#include "huffman.h"
#include "defines.h"

#include "io.h"

#include "stack.h"
#include "pq.h"
#include "node.h"
#include "code.h"

static uint8_t buffer[BLOCK];

void traversal(Node *n, Code *c, Code table[ALPHABET]) {
    printf("running traversal\n");
    if (n->left == NULL && n->right == NULL) {
        printf("found a leaf\n");
        table[n->symbol] = *c;
        printf("%d ", n->symbol);
        code_print(c);
        return;
    } else {
        printf("found a parent\n");
        uint8_t bit;
        code_push_bit(c, 0);
        if (n->left) {
            traversal(n->left, c, table);
        }
        code_pop_bit(c, &bit);
        code_push_bit(c, 1);
        if (n->right) {
            traversal(n->right, c, table);
        }
        code_pop_bit(c, &bit);
        return;
    }
}

Node *build_tree(uint64_t hist[static ALPHABET]) {

    printf("running build tree\n");

    PriorityQueue *pq = pq_create(ALPHABET);
    printf("getting to the for loop\n");
    for (int i = 0; i < ALPHABET; i += 1) {

        if (hist[i] == 0) {
            continue;
        } else {
            //printf("i: %d\n", i);
            Node *n = node_create((char) i, hist[i]);

            enqueue(pq, n);
        }
    }
    printf("priority queue:\n");
    pq_print(pq);
    Node *joined = NULL;
    while (pq_size(pq) != 1) {
        //printf("pq size, %d\n", pq_size(pq));
        Node *left = node_create('$', 0);
        Node *right = node_create('$', 0);
        dequeue(pq, &left);
        dequeue(pq, &right);
        joined = node_join(left, right);
        //node_print(joined);
        enqueue(pq, joined);
    }

    dequeue(pq, &joined);
    //pq_print(pq);
    pq_delete(&pq);
    printf("tree\n");
    node_print(joined);
    return joined;
}

void build_codes(Node *root, Code table[static ALPHABET]) {
    Code c = code_init();

    if (root) {
        traversal(root, &c, table);
    }
    return;
}

void dump_tree(int outfile, Node *root) {
    if (root) {
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);
        if (!(root->left) && !(root->right)) {
            printf("L");
            write_bytes(outfile, buffer, 'L');
            printf("%c", (char) root->symbol);
            write_bytes(outfile, buffer, (char) root->symbol);
        }
    } else {
        printf("I");
        write_bytes(outfile, buffer, 'I');
    }
}

void delete_tree(Node **root) {
    if (*root) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->left);
        node_delete(root);
    } else {
        return;
    }
}
