#include "heap.h"

#include "stats.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

// credit to Professor Long for python example code

// finds the largest child value from a parent node.
// returns largest child.
//
// stats: from the stats struct in stats.h, used to count compares and moves
// A: array that is to be sorted
// first: parent node
// last: last index of the array
uint32_t max_child(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if ((right <= last) && (cmp(stats, A[right - 1], A[left - 1]) > 0)) {
        return right; // right child is largest value
    }
    return left; // left child is largest value
}

// fixes a heap into a max heap.
// returns void.
//
// stats: used to count compares and moves
// A: array that is to be sorted
// first: parent node of array
// last: last index of array
void fix_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t mother = first;
    uint32_t great = max_child(stats, A, mother, last);

    while ((mother <= floor(last / 2.0)) && !(found)) {
        if (cmp(stats, A[great - 1], A[mother - 1]) > 0) { // if largest child > mother
            swap(stats, &A[mother - 1], &A[great - 1]); // swap them
            mother = great;
            great = max_child(stats, A, mother, last); // check the next children
        } else {
            found = true; // mother > largest child
        }
    }
    return;
}

// builds a heap into a max heap.
// returns void.
//
// stats: used to count compares and moves
// A: array that is to be sorted
// first: index of 1
// last: last index of array
void build_heap(Stats *stats, uint32_t *A, uint32_t first, uint32_t last) {
    for (uint32_t father = floor(last / 2.0); father > first - 1; father -= 1) {
        fix_heap(stats, A, father, last); // creates a max heap
    }
    return;
}

// sorts an array using heap sort method.
// returns void.
//
// stats: used to count compares and moves
// A: array that is to be sorted
// n: length of the array
void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    reset(stats);
    uint32_t first = 1;
    uint32_t last = n;
    build_heap(stats, A, first, last); // creates a max heap with initial values
    for (uint32_t leaf = last; leaf > first; leaf -= 1) {
        swap(stats, &A[first - 1], &A[leaf - 1]);
        fix_heap(stats, A, first, leaf - 1); // fixes heap until array is ordered
    }
    return;
}
