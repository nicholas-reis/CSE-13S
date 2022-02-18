/* contains the implementation of the Newton-Raphson method to approximate the
 * square root of a number and the function to return the number of iterations */

#include "mathlib.h"

#include <stdio.h>

static int iters = 0;

double sqrt_newton(double num) {
    // credit to Professor Long for following code
    iters = 0;
    double z = 0.0;
    double y = 1.0;
    double x = num;

    while (absolute(y - z) > EPSILON) {
        z = y;
        y = 0.5 * (z + (x / z));
        iters += 1;
    }
    return y;
}

int sqrt_newton_iters(void) {
    return iters;
}
