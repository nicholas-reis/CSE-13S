#include "quick.h"

#include "stats.h"

#include <stdint.h>
#include <stdio.h>

// credit to Professor Long for python example code.

// creates a partition and locates position for pivot.
// returns pivot index.
//
// stats: from the stats struct in stats.h, used to count compares and moves
// A: array that is to be sorted
// lo: low index of partition
// hi: high index of partition
uint32_t partition(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    uint32_t i = lo - 1;
    for (uint32_t j = lo; j < hi; j += 1) {
        if (cmp(stats, A[hi - 1], A[j - 1]) > 0) { // compares each value to pivot
            i += 1;
            swap(stats, &A[j - 1], &A[i - 1]); // puts value to the left half if smaller than pivot
        }
    }
    swap(stats, &A[i], &A[hi - 1]); // pivot placed after all values smaller than it
    return i + 1;
}

// recursively sorts each partition of the array.
// returns void.
//
// stats: used to count compares and moves
// A: array that is to be sorted
// lo: low index of partition
// hi: high index of partition
void quick_sorter(Stats *stats, uint32_t *A, uint32_t lo, uint32_t hi) {
    if (lo < hi) {
        uint32_t p = partition(stats, A, lo, hi); // gets index of partition
        quick_sorter(stats, A, lo, p - 1); // sorts left half of partition recursively
        quick_sorter(stats, A, p + 1, hi); // sorts right half of partition recursively
    }
    return;
}

// sorts an array using the quick sort method.
// returns void.
//
// stats: used to count compares and moves
// A: array that is to be sorted
// n: length of the array
void quick_sort(Stats *stats, uint32_t *A, uint32_t n) {
    reset(stats);
    quick_sorter(stats, A, 1, n);
    return;
}
