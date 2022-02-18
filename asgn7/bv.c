#include "bv.h"

#include <stdio.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

// Constructor for a BitVector of length bits.
// Returns the initialized BitVector.
//
// length: number of bits the BitVector can contain
BitVector *bv_create(uint32_t length) {
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if (bv) {
        bv->length = length;
        // Allocate memory for the bits
        bv->vector = (uint8_t *) calloc(length, sizeof(uint8_t));
    } else {
        free(bv);
        bv = NULL;
    }
    return bv;
}

// Destructor for a BitVector. Free memory allocated to bits
// and the bv and sets the pointer to NULL.
//
// bv: free this BitVector
void bv_delete(BitVector **bv) {
    if (*bv && (*bv)->vector) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }
    return;
}

// Returns the length of a given BitVector.
//
// bv: given BitVector
uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

// Credit to Prof. Long from Code Comments Repo
// Sets the bit at index i to 1. Returns false if i is
// out of range, true otherwise.
//
// bv: BitVector that is altered
// i: index to set bit
bool bv_set_bit(BitVector *bv, uint32_t i) {
    if (i >= bv->length) {
        return false;
    }
    // Sets the bit by or'ing with left shifted 1 bit vector
    bv->vector[i / 8] |= (0x1 << i % 8);
    return true;
}

// Credit to Prof. Long from Code Comments Repo
// Clears the bit at index i to 0. Returns false if i
// is out of range, true otherwise.
//
// bv: BitVector that is altered
// i: index to clear bit
bool bv_clr_bit(BitVector *bv, uint32_t i) {
    if (i >= bv->length) {
        return false;
    }
    // Clears the bit by and'ing with inverse bit vector
    bv->vector[i / 8] &= ~(0x1 << i % 8);
    return true;
}

// Credit to Prof. Long from Code Comments Repo
// Checks if the bit at index i is 0 or out of range, and
// returns false if it is, returns true if the bit is 1.
//
// bv: pointer to the Code
// i: index to get bit
bool bv_get_bit(BitVector *bv, uint32_t i) {
    if (i >= bv->length || ((bv->vector[i / 8] >> i % 8) & 0x1) == 0x0) {
        return false;
    }
    return true;
}

// Print out the bits of a given BitVector.
//
// bv: given BitVector
void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; i += 1) {
        printf("bit %u: %u\n", i, bv->vector[i]);
    }
}
