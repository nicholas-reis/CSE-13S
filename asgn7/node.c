#include "node.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Constructor for Node that sets the Node's oldspeak and
// newspeak translations and the left and right
// children to NULL. Return the node.
//
// oldspeak: oldspeak translation of the node
// newspeak: newspeak translation of the node
Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));
    if (n && oldspeak) {
        // Copy the oldspeak string
        n->oldspeak = strdup(oldspeak);
        // There may not be a newspeak translation
        if (newspeak) {
            n->newspeak = strdup(newspeak);
        } else {
            n->newspeak = NULL;
        }
        n->left = NULL;
        n->right = NULL;
    } else {
        free(n);
        n = NULL;
    }
    return n;
}

// Destructor for Node that frees the memory of the Node
// and oldspeak/newspeak before setting the pointer to NULL.
//
// n: Node to be deleted
void node_delete(Node **n) {
    if (*n) {
        free((*n)->oldspeak);
        free((*n)->newspeak);
        free(*n);
        *n = NULL;
    }
    return;
}

// Credit to Prof. Long from assignment 7 pdf
// Debug function to print out oldspeak and newspeak
// translation, or just oldspeak if there is no
// newspeak.
void node_print(Node *n) {
    if (n) {
        if (n->oldspeak && n->newspeak) {
            printf("%s -> %s\n", n->oldspeak, n->newspeak);
        } else if (n->oldspeak) {
            printf("%s\n", n->oldspeak);
        }
    }
    return;
}
