/* contains the implementation of Viete's formula to approximate pi
 * and the function to return the number of computed iterations */

#include "mathlib.h"

#include <stdio.h>

static int iters = 0;

double pi_viete(void) {
    iters = 0;
    double term = sqrt_newton(2.0);
    double pi_approx = term / 2.0;
    double previous_approx = 0.0;
    double diff = 1.0;

    for (int i = 0; diff >= EPSILON; i++) {
        previous_approx = pi_approx;
        term = sqrt_newton(2.0 + term);
        pi_approx *= (term / 2.0);

        diff = absolute(pi_approx - previous_approx);
        iters += 1;
    }
    return (2.0 / pi_approx);
}

int pi_viete_factors(void) {
    return iters;
}
