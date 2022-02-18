#include "node.h"

#include <stdlib.h>
#include <stdio.h> // only used for debugging

// Constructor for Node that sets the Node's symbol to
// symbol, frequency to frequency, and left and right
// children to NULL. Return the node.
//
// symbol: symbol of the node
// frequency: how many times the symbol appears
Node *node_create(uint8_t symbol, uint64_t frequency) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n) {
        n->symbol = symbol;
        n->frequency = frequency;
        n->left = NULL;
        n->right = NULL;
    } else {
        free(n);
        n = NULL;
    }
    return n;
}

// Destructor for Node that frees the memory of the Node
// and then sets the pointer to NULL.
//
// n: Node to be deleted
void node_delete(Node **n) {
    if (*n) {
        free(*n);
        *n = NULL;
    }
    return;
}

// Joins left and right node and returns pointer to a new
// parent node. Left child is left and right child is right.
// New node will have the '$' symbol and a frequency that
// is equal to the sum of the left and right nodes.
//
// left: left child node
// right: right child node
Node *node_join(Node *left, Node *right) {
    uint64_t freq = left->frequency + right->frequency;
    Node *n = node_create('$', freq);
    n->left = left;
    n->right = right;
    return n;
}

// Debug function
void node_print(Node *n) {
    printf("Node frequency: %lu\n", n->frequency);
    printf("Node symbol: %uuu\n", n->symbol);
    printf("Left child frequency and symbol: %lu %uuu\n", (n->left)->frequency, (n->left)->symbol);
    printf(
        "Right child frequency and symbol: %lu %uuu\n", (n->right)->frequency, (n->right)->symbol);
}
