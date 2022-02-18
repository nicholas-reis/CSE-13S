#include "huffman.h"
#include "defines.h"
#include "pq.h"
#include "io.h"
#include "stack.h"

#include <stdio.h>
#include <unistd.h>

static uint32_t symbols;
// Credit to Prof. Long in the assignment 5 pdf
// Uses a priority queue to store all the inputted symbols in nodes,
// and then dequeues two nodes at a time, joins them, and does this
// until there is only one node left, which is the returned root node.
//
// hist[]: histogram with each symbols' frequency
Node *build_tree(uint64_t hist[static ALPHABET]) {
    PriorityQueue *tree = pq_create(ALPHABET);
    // Enqueue every item in the histogram with a frequency > 0
    for (uint32_t i = 0; i < ALPHABET; i += 1) {
        if (hist[i] > 0) {
            Node *node = node_create(i, hist[i]);
            enqueue(tree, node);
            symbols += 1;
        }
    }
    // Dequeue two nodes and join them into one
    Node *left;
    Node *right;
    Node *root;
    while (pq_size(tree) > 1) {
        dequeue(tree, &left);
        dequeue(tree, &right);
        Node *parent = node_join(left, right);
        enqueue(tree, parent);
    }
    dequeue(tree, &root);
    pq_delete(&tree);
    // Return root node
    return root;
}

static Code c;
static bool init = true;
// Credit to Prof. Long in the assignment 5 pdf
// Creates a code for each symbol using a post-order traversal
// and stores the respective codes in the table array.
//
// root: start at the root of the tree
// table: array that will store each symbols' code
void build_codes(Node *root, Code table[static ALPHABET]) {
    if (init) {
        c = code_init();
        init = false;
    }
    uint8_t temp;
    // Post order traversal starting from root of the tree
    if (root != NULL) {
        if (root->left == NULL && root->right == NULL) {
            // Once a leaf is reached, store the code in the table
            table[root->symbol] = c;
        } else {
            // Go down the left side of the tree by pushing a 0
            code_push_bit(&c, 0);
            build_codes(root->left, table);
            code_pop_bit(&c, &temp);

            // Go down the right side of the tree by pushing a 1
            code_push_bit(&c, 1);
            build_codes(root->right, table);
            code_pop_bit(&c, &temp);
        }
    }
}

static uint8_t buf[BLOCK] = { 0 };
static uint32_t counter = 0;
// Credit to Prof. Long in the assignment 5 pdf
// Uses post-order traversal to buffer an 'L' and a node's symbol
// if a leaf node is reached, or an 'I' if an interior node is
// reached. Once the amount of symbols buffer equals the tree
// size, write out the buffer to outfile.
//
// outfile: print to this output file
// root: start at the root of the tree
void dump_tree(int outfile, Node *root) {
    if (root) {
        // Post order traversal
        dump_tree(outfile, root->left);
        dump_tree(outfile, root->right);

        if (root->left == NULL && root->right == NULL) {
            // If a leaf node is reached, buffer L and the symbol
            buf[counter] = 76;
            counter += 1;
            buf[counter] = root->symbol;
            counter += 1;
        } else {
            // Interior node is buffered
            buf[counter] = 73;
            counter += 1;
        }
    }
    // If counter equals tree size, write the buffer and set counter to 0
    if (counter == 3 * symbols - 1) {
        write_bytes(outfile, buf, counter);
        counter = 0;
    }
}

// Iterates through the dumped tree and creates nodes for
// each leaf. Uses a stack to store each node. Returns
// the root of the tree.
//
// nbytes: length of the array tree_dump
// tree_dump: array with the entire tree dump
Node *rebuild_tree(uint16_t nbytes, uint8_t tree_dump[static nbytes]) {
    Stack *node_stack = stack_create(nbytes);
    for (uint16_t i = 0; i < nbytes; i += 1) {
        // If the symbol is L, create a node for the leaf and push to stack
        if (tree_dump[i] == 'L') {
            // Jump to the leaf node
            i += 1;
            Node *leaf = node_create(tree_dump[i], 0);
            stack_push(node_stack, leaf);
            // If the symbol is I, pop left and right nodes and create a parent
        } else {
            Node *right;
            Node *left;
            stack_pop(node_stack, &right);
            stack_pop(node_stack, &left);
            Node *parent = node_join(left, right);
            stack_push(node_stack, parent);
        }
    }
    // Pop the root node and return it
    Node *root;
    stack_pop(node_stack, &root);
    stack_delete(&node_stack);
    return root;
}

// Delete each node in the tree by freeing them from memory
//
// root: start from the root node
void delete_tree(Node **root) {
    if (*root) {
        delete_tree(&(*root)->left);
        delete_tree(&(*root)->right);
        node_delete(root);
    }
}
