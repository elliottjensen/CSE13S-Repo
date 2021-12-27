#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "bst.h"
#include "node.h"

uint64_t branches = 0; // initialize branches for the sake of statistics

// Returns the maximum of two integers
// taken from slide 18: Daryll Long page 55
static int max(int x, int y) {
    return x > y ? x : y;
}

// Creates an empty tree
// Returns NULL
Node *bst_create(void) {
    return NULL;
}

// Deletes a binary search tree through a post order traversal
// taken from slide 18: Daryl Long page 85
void bst_delete(Node **root) {
    if (*root) {
        if ((*root)->left) {
            bst_delete(&(*root)->left);
        }
        if ((*root)->right) {
            bst_delete(&(*root)->right);
        }
        node_delete(root);
    }
    return;
}

// Calculates the height of a binary search tree
// Returns the hieght of the binary search tree
// root: the root of the binary search tree whose height will be returned
// taken from slide 18: Daryll Long page 55
uint32_t bst_height(Node *root) {
    if (root) { // makes sure that the binary search tree exists before
        return 1 + max(bst_height(root->left), bst_height(root->right));
    }
    return 0; // the height of an empty bst is 0
}

// Calculates the size of a binary search tree
// Returns an integer that is the size of the binary search tree
// root: the root of the binary search tree
uint32_t bst_size(Node *root) {
    if (root) { // makes sure that the bst exists
        return 1 + bst_size(root->left) + bst_size(root->right);
    }
    return 0; // the size of the empty bst is 0
}

// Checks if oldspeak is in the bst
// Returns the node if it is in the bst, otherwise returns root
// root: the root of the bst
// oldspeak: the word to be searched for in the bst
// taken from slide 18: Darryl Long, page 57
Node *bst_find(Node *root, char *oldspeak) {
    if (root) { // makes sure that the bst exists
        // uses strcmp to look in the right direction
        if (strcmp(root->oldspeak, oldspeak) > 0) {
            return bst_find(root->left, oldspeak);
        } else if (strcmp(root->oldspeak, oldspeak) < 0) {
            return bst_find(root->right, oldspeak);
        }
    }

    branches += 1;
    return root;
}

// Inserts a node into a binary search tree
// If the insertion is successful this function returns the node inserted. Otherwise it
// returns the root
// root: the root of the bst
// oldspeak: the oldspeak stored in the node to be inserted
// newspeak: the newspeak stored in the ndoe to be isnerted
// taken from slide 18: Darryl Long, page 62 (changed a bit)
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (root == NULL) { // If the current node doesnt exist, insert the node here
        return node_create(oldspeak, newspeak);
    }
    if (bst_find(root, oldspeak) == NULL) { // if the node is a duplicate return root
        if (root) { // if the root exists, begin traversing the bst
            // use strcmp to go in the right direction
            if (strcmp(root->oldspeak, oldspeak) > 0) {
                root->left = bst_insert(root->left, oldspeak, newspeak);
            } else if (strcmp(root->oldspeak, oldspeak) < 0) {
                root->right = bst_insert(root->right, oldspeak, newspeak);
            }
            return root;
        }
        return node_create(oldspeak, newspeak);
    }
    return root;
}

// Prints the binary search tree
// root: the root of the binary search tree to be printed
// taken from slide 18: Darryl Long, page 22
void bst_print(Node *root) {
    if (root) { // makes sure the root exists before print it
        // uses an inorder traversal
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
    return;
}
