#include "bst.h"
#include "node.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

uint64_t branches = 0;

// Constructs an empty binary search tree, meaning
// the tree is simply NULL.
Node *bst_create(void) {
    return NULL;
}

// Same code from my huffman delete_tree() function.
// Performs post-order traversal to delete each node
// in a binary search tree.
//
// root: root node of the tree
void bst_delete(Node **root) {
    if (*root) {
        bst_delete(&(*root)->left);
        bst_delete(&(*root)->right);
        node_delete(root);
    }
    return;
}

// Helper function to determine maximum value
// between two integers. Returns the larger number.
//
// a and b: integers that are compared
uint32_t maximum(uint32_t a, uint32_t b) {
    if (b > a) {
        return b;
    } else {
        return a;
    }
}

// Credit to Prof. Long for this code
// Returns the height of the bst starting from root
//
// root: root node of the tree
uint32_t bst_height(Node *root) {
    // Recursive calls to see if left or right side is taller
    if (root) {
        return 1 + maximum(bst_height(root->left), bst_height(root->right));
    }
    return 0;
}

// Returns the number of nodes in a bst.
//
// root: root node of the tree.
uint32_t bst_size(Node *root) {
    // Counts every node in the tree
    if (root) {
        return 1 + bst_size(root->left) + bst_size(root->right);
    }
    return 0;
}

// Credit to Prof. Long for this code
// Searches for a node with a specific oldspeak string, returns
// a pointer to the node if found and a NULL pointer if not.
// Increment branches traversed for each recursive call.
//
// root: root node of the tree
// oldspeak: search for a node with this string
Node *bst_find(Node *root, char *oldspeak) {
    if (root) {
        // If oldspeak is less than root's oldspeak, go left down the tree
        if (strcasecmp(root->oldspeak, oldspeak) > 0) {
            branches += 1;
            return bst_find(root->left, oldspeak);
            // Otherwise go right down the tree
        } else if (strcasecmp(root->oldspeak, oldspeak) < 0) {
            branches += 1;
            return bst_find(root->right, oldspeak);
        }
    }
    // Will either return node if found, NULL if not
    return root;
}

// Credit to Prof. Long for this code
// Inserts a new node into a bst starting from root.
// Returns the root node of the bst. Increment branches
// traversed for each recursive call.
//
// root: root node of the tree
// oldspeak and newspeak: new node contains these strings
Node *bst_insert(Node *root, char *oldspeak, char *newspeak) {
    if (root) {
        // If oldspeak is less than root's oldspeak, go left down the tree
        if (strcasecmp(root->oldspeak, oldspeak) > 0) {
            branches += 1;
            root->left = bst_insert(root->left, oldspeak, newspeak);
            // If oldspeak is greater than root's oldspeak, go right down the tree
        } else if (strcasecmp(root->oldspeak, oldspeak) < 0) {
            branches += 1;
            root->right = bst_insert(root->right, oldspeak, newspeak);
        }
        return root;
    }
    // Insert the node once a NULL node is reached.
    return node_create(oldspeak, newspeak);
}

// Performs an inorder traversal to print out each node in a tree.
//
// root: root node of the tree
void bst_print(Node *root) {
    if (root) {
        bst_print(root->left);
        node_print(root);
        bst_print(root->right);
    }
    return;
}
