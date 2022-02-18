/* contains the implementation of Euler's solution to approximate pi
 * and the function to return the number of computed terms */

#include "mathlib.h"

#include <stdio.h>

static int iters = 1; // start at 1 to avoid first iteration difficulties

double square(double x) {
    return (x * x);
}

double pi_euler(void) {
    iters = 1; // in case function is run multiple times
    double pi_approx = 1.0; // first term is 1.0
    double term = 1.0;
    double k = 2.0;

    for (int i = 1; term >= EPSILON; i++) {
        term = 1.0 / (square(k));
        pi_approx += term;
        iters += 1;
        k += 1.0;
    }
    return sqrt_newton(pi_approx * 6.0); // need to do this to get value of pi
}

int pi_euler_terms(void) {
    return iters;
}
