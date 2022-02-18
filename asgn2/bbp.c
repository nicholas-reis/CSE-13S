/* contains implementation of the Bailey-Borwein-Plouffe formula to approximate pi
 * and the function to return the number of computed terms. */

#include "mathlib.h"

#include <stdio.h>

static int iters = 0;

double subtractions(int x) {
    double fraction1 = (4.0 / ((8.0 * x) + 1.0));
    double fraction2 = (2.0 / ((8.0 * x) + 4.0));
    double fraction3 = (1.0 / ((8.0 * x) + 5.0));
    double fraction4 = (1.0 / ((8.0 * x) + 6.0));
    return (fraction1 - fraction2 - fraction3 - fraction4);
}

double pi_bbp(void) {
    iters = 0;
    double term = 1.0;
    double pi_approx = 0.0;
    double divider = 1.0;

    for (int i = 0; term >= EPSILON; i++) {
        if (i == 0) {
            term = subtractions(0);
            pi_approx = term;
            iters += 1;
        } else {
            divider *= 16.0;
            term = (subtractions(i) / divider);
            pi_approx += term;
            iters += 1;
        }
    }
    return pi_approx;
}

int pi_bbp_terms(void) {
    return iters;
}
