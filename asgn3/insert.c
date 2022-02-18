#include "insert.h"

#include "stats.h"

#include <stdint.h>
#include <stdio.h>

// credit to Professor Long for python example code.
// sorts an array using insertion sort method.
// returns void.
//
// stats: from the stats struct in stats.h, used to count compares and moves
// A: array that is to be sorted
// n: length of the array
void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {
    reset(stats); // resets all stats to 0
    for (uint32_t i = 1; i < n; i += 1) {
        uint32_t j = i; // begin at second element of array
        uint32_t temp = move(stats, A[i]);

        while (j > 0 && cmp(stats, A[j - 1], temp) > 0) { // while previous number > current number
            A[j] = move(stats, A[j - 1]);
            j -= 1;
        }
        A[j] = move(stats, temp);
    }
    return;
}
