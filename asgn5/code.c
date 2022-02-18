#include "code.h"

#include "defines.h"
#include <stdio.h> // only used for debugging

// Creates a new Code on the stack, sets top to 0,
// and zeroes out the bits array.
// Returns the initialized Code.
Code code_init(void) {
    // Credit to Christian
    Code c = { .top = 0, .bits = { 0 } };
    return c;
}

// Returns the size of the Code (number of bits pushed
// into the Code)
//
// *c: pointer to the Code
uint32_t code_size(Code *c) {
    return c->top;
}

// Returns true if the Code is empty, false otherwise
//
// *c: pointer to the Code
bool code_empty(Code *c) {
    if (c->top == 0) {
        return true;
    }
    return false;
}

// Returns true if the Code is full, false otherwise
//
// *c: pointer to the Code
bool code_full(Code *c) {
    if (c->top == ALPHABET) {
        return true;
    }
    return false;
}

// Credit to Prof. Long from Code Comments Repo
// Sets the bit at index i to 1. Returns false if i is
// out of range, true otherwise.
//
// *c: pointer to the Code
// i: index to set bit
bool code_set_bit(Code *c, uint32_t i) {
    if (i >= ALPHABET) {
        return false;
    }
    // Sets the bit by or'ing with left shifted 1 bit vector
    c->bits[i / 8] |= (0x1 << i % 8);
    return true;
}

// Credit to Prof. Long from Code Comments Repo
// Clears the bit at index i to 0. Returns false if i
// is out of range, true otherwise.
//
// *c: pointer to the Code
// i: index to clear bit
bool code_clr_bit(Code *c, uint32_t i) {
    if (i >= ALPHABET) {
        return false;
    }
    // Clears the bit by and'ing with inverse bit vector
    c->bits[i / 8] &= ~(0x1 << i % 8);
    return true;
}

// Credit to Prof. Long from Code Comments Repo
// Checks if the bit at index i is 0 or out of range, and
// returns false if it is, returns true if the bit is 1.
//
// *c: pointer to the Code
// i: index to get bit
bool code_get_bit(Code *c, uint32_t i) {
    if (i >= ALPHABET || ((c->bits[i / 8] >> i % 8) & 0x1) == 0x0) {
        return false;
    }
    return true;
}

// Pushes a bit onto the Code. Returns false if the Code
// is full before the bit is pushed, true otherwise.
//
// *c: pointer to the Code
// bit: value of pushed bit
bool code_push_bit(Code *c, uint8_t bit) {
    if (code_full(c)) {
        return false;
    }
    if (bit == 0) {
        code_clr_bit(c, c->top);
    } else if (bit == 1) {
        code_set_bit(c, c->top);
    }
    c->top++;
    return true;
}

// Pops a bit from the Code. Returns false if the Code
// is empty before the bit is popped, true otherwise.
//
// *c: pointer to the Code
// *bit: value of popped bit is passed back with this
bool code_pop_bit(Code *c, uint8_t *bit) {
    if (code_empty(c)) {
        return false;
    }
    c->top--;
    if (code_get_bit(c, c->top)) {
        *bit = 1;
    } else {
        *bit = 0;
    }
    return true;
}

// Debug function for Code
void code_print(Code *c) {
    printf("top: %u\n", c->top);
    for (uint32_t i = 0; i < c->top; i += 1) {
        printf("bit %u: %uuu\n", i, c->bits[i]);
    }
}
