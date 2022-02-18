#include "bf.h"
#include "bv.h"
#include "salts.h"
#include "speck.h"

#include <stdlib.h>
#include <stdio.h>

struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

// Constructor for BloomFilter that defines the salts,
// sets the filter's size to size, and returns the BloomFilter.
//
// size: size of filter
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));
    if (bf) {
        bf->primary[0] = SALT_PRIMARY_LO;
        bf->primary[1] = SALT_PRIMARY_HI;
        bf->secondary[0] = SALT_SECONDARY_LO;
        bf->secondary[1] = SALT_SECONDARY_HI;
        bf->tertiary[0] = SALT_TERTIARY_LO;
        bf->tertiary[1] = SALT_TERTIARY_HI;
        bf->filter = bv_create(size);
    } else {
        free(bf);
        bf = NULL;
    }
    return bf;
}

// Destructor for BloomFilter, free the filter of bf, along with
// the bf itself and the pointer is set to NULL.
//
// bf: BloomFilter to be deleted
void bf_delete(BloomFilter **bf) {
    if (*bf) {
        bv_delete(&(*bf)->filter);
        free(*bf);
        *bf = NULL;
    }
    return;
}

// Returns the size of a given BloomFilter.
//
// bf: given BloomFilter
uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

// Inserts oldspeak into the given BloomFilter. To do this,
// hash oldspeak with each salt and set the bits at those indices.
//
// bf: given BloomFilter
// oldspeak: oldspeak string to be inserted
void bf_insert(BloomFilter *bf, char *oldspeak) {
    uint32_t bit = 0;
    bit = hash(bf->primary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, bit);
    bit = hash(bf->secondary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, bit);
    bit = hash(bf->tertiary, oldspeak) % bf_size(bf);
    bv_set_bit(bf->filter, bit);
    return;
}

// If oldspeak is not in all of the indices given by the hash
// calls, oldspeak is not in the BloomFilter so return false.
// Otherwise, it might be in the bf, so return true.
//
// bf: BloomFilter that is probed
// oldspeak: oldspeak string that is probed for
bool bf_probe(BloomFilter *bf, char *oldspeak) {
    uint32_t bit = hash(bf->primary, oldspeak) % bf_size(bf);
    if (!bv_get_bit(bf->filter, bit)) {
        return false;
    }
    bit = hash(bf->secondary, oldspeak) % bf_size(bf);
    if (!bv_get_bit(bf->filter, bit)) {
        return false;
    }
    bit = hash(bf->tertiary, oldspeak) % bf_size(bf);
    if (!bv_get_bit(bf->filter, bit)) {
        return false;
    }
    return true;
}

// Returns the total number of set bits in a given BloomFilter.
//
// bf: given BloomFilter
uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;
    for (uint32_t i = 0; i < bf_size(bf); i += 1) {
        if (bv_get_bit(bf->filter, i)) {
            count += 1;
        }
    }
    return count;
}

// Prints out the bit vector of a BloomFilter.
//
// bf: Bloomfilter that is printed
void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);
    return;
}
