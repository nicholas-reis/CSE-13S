#include "shell.h"

#include "stats.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>

static uint32_t gap = 0;

// credit to Professor Long for python example code.
// sorts an array using the shell sort method.
// returns void.
//
// stats: from the stats struct in stats.h, used to count compares and moves
// A: array that is to be sorted
// n: length of the array
void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {
    reset(stats); // reset all stats to 0
    gap = 0;
    double temp = (log(3.0 + 2.0 * n) / log(3.0)); // max value for k in gap calculation
    int max = temp;
    for (uint32_t i = max; i > 0; i -= 1) {
        gap = floor((pow(3.0, i) - 1.0) / 2.0); // calculate gap size with decrementing k value
        for (uint32_t i = gap; i < n; i++) {
            uint32_t j = i;
            uint32_t temp = move(stats, A[i]);
            while (j >= gap && cmp(stats, A[j - gap], temp) > 0) {
                A[j] = move(stats, A[j - gap]);
                j -= gap;
            }
            A[j] = move(stats, temp);
        }
    }
    return;
}
