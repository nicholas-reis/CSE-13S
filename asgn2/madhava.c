/* contains implementation of the Madhava series to approximate pi
 * and the function to return the number of computed terms */

#include "mathlib.h"

#include <stdio.h>

static int iters = 0;

double pi_madhava(void) {
    iters = 0; // in case function is run multiple times
    double term = 1.0;
    double pi_approx = 0.0;
    double divider = 1.0;

    for (int i = 0; absolute(term) >= EPSILON; i++) {
        if (i == 0) {
            term = 1.0;
            pi_approx = term;
            iters += 1;
        } else {
            divider *= -3.0; // not necessary to use exponent function
            term = ((1.0 / (((2.0 * i) + 1.0) * divider)));
            pi_approx += term;
            iters += 1;
        }
    }
    return (sqrt_newton(12) * pi_approx); // multiply by sqrt(12) to get pi
}

int pi_madhava_terms(void) {
    return iters;
}
