#include "ht.h"
#include "bst.h"
#include "node.h"
#include "salts.h"
#include "speck.h"

#include <stdlib.h>
#include <stdio.h>

uint64_t lookups = 0;

struct HashTable {
    uint64_t salt[2];
    uint32_t size;
    Node **trees;
};

// Constructor for HashTable that sets the HashTable size to
// size, creates size number of bst's, defines the salts, and
// returns the HashTable.
//
// size: size of HashTable
HashTable *ht_create(uint32_t size) {
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht) {
        ht->size = size;
        // Allocate memory for the bst's
        ht->trees = (Node **) calloc(size, sizeof(Node *));
        for (uint32_t i = 0; i < size; i += 1) {
            ht->trees[i] = bst_create();
        }
        ht->salt[0] = SALT_HASHTABLE_LO;
        ht->salt[1] = SALT_HASHTABLE_HI;
    } else {
        free(ht);
        ht = NULL;
    }
    return ht;
}

// Destructor for Hashtable, free each tree in the ht, along with
// the ht itself and the pointer is set to NULL.
//
// ht: HashTable to be deleted
void ht_delete(HashTable **ht) {
    if (*ht) {
        for (uint32_t i = 0; i < (*ht)->size; i += 1) {
            bst_delete(&(*ht)->trees[i]);
        }
        free((*ht)->trees);
        free(*ht);
        *ht = NULL;
    }
    return;
}

// Returns the size of a given HashTable.
//
// ht: given HashTable
uint32_t ht_size(HashTable *ht) {
    return ht->size;
}

// Searches for a specific node in the HashTable that has
// the oldspeak string in it. Returns the node if found,
// otherwise returns a NULL pointer. Increment lookups
// each time the function is called.
//
// ht: HashTable that is searched
// oldspeak: string that is searched for
Node *ht_lookup(HashTable *ht, char *oldspeak) {
    lookups += 1;
    uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);
    return bst_find(ht->trees[index], oldspeak);
}

// Inserts an oldspeak-newspeak translation into the HashTable.
// Increment lookups each time the function is called.
//
// ht: insert into this HashTable
// oldspeak: oldspeak string that is inserted
// newspeak: newspeak string that is inserted
void ht_insert(HashTable *ht, char *oldspeak, char *newspeak) {
    lookups += 1;
    uint32_t index = hash(ht->salt, oldspeak) % ht_size(ht);
    // Need to update the tree after inserting the translation
    ht->trees[index] = bst_insert(ht->trees[index], oldspeak, newspeak);
    return;
}

// Returns the number of non-NULL bst's in the given HashTable.
//
// ht: HashTable that is looked through
uint32_t ht_count(HashTable *ht) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if (ht->trees[i] != NULL) {
            count += 1;
        }
    }
    return count;
}

// Returns the average size of the bst's in a given HashTable.
//
// ht: HashTable that is looked through
double ht_avg_bst_size(HashTable *ht) {
    double total_size = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if (ht->trees[i] != NULL) {
            total_size += bst_size(ht->trees[i]);
        }
    }
    return total_size / (double) ht_count(ht);
}

// Returns the average height of the bst's in a given HashTable.
//
// ht: HashTable that is looked through
double ht_avg_bst_height(HashTable *ht) {
    double total_size = 0;
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if (ht->trees[i] != NULL) {
            total_size += bst_height(ht->trees[i]);
        }
    }
    return total_size / (double) ht_count(ht);
}

// Prints out the contents of a given HashTable.
//
// ht: HashTable that is printed
void ht_print(HashTable *ht) {
    if (ht) {
        for (uint32_t i = 0; i < ht->size; i += 1) {
            if (ht->trees[i] != NULL) {
                bst_print(ht->trees[i]);
            }
        }
    }
    return;
}
