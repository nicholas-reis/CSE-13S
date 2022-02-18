/* contains the implementation of the Taylor series to approximate Euler's number e and
 * the function to return the number of computed terms. */

#include "mathlib.h"

#include <stdio.h>

static int iters = 0;

double e(void) {
    iters = 0; // in case function is run multiple times
    double e_approx = 0.0;
    double term = 1.0;
    double k = 0.0;

    for (int i = 0; term >= EPSILON; i++) {
        if ((i == 0) || (i == 1)) {
            k = 1.0;
        } else {
            k += 1.0;
        }
        double multiplier = 1.0 / k;
        term *= multiplier; // next term is current term * (x/k)
        e_approx += term; // add value to approximation
        iters += 1;
    }
    return e_approx;
}

int e_terms(void) {
    return iters;
}
