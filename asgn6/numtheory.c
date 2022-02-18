#include "numtheory.h"
#include "randstate.h"

#include <stdlib.h>

gmp_randstate_t state;

// Computes the greatest common divisor between a and b,
// and stores the value in g.
//
// g: store computed result here
// a and b: find gcd of these two numbers
void gcd(mpz_t g, mpz_t a, mpz_t b) {
    // Create copies of a and b to preserve their value
    mpz_t temp, a_copy, b_copy;
    mpz_inits(temp, a_copy, b_copy, NULL);
    mpz_set(a_copy, a);
    mpz_set(b_copy, b);
    // While b_copy does not equal 0
    // temp preserves the original value of b_copy
    // a_copy = b_copy
    // b_copy = a_copy % b_copy
    while (mpz_cmp_ui(b_copy, 0) != 0) {
        mpz_set(temp, b_copy);
        mpz_mod(b_copy, a_copy, b_copy);
        mpz_set(a_copy, temp);
    }
    // Set g to computed gcd (a_copy)
    mpz_set(g, a_copy);
    mpz_clears(temp, a_copy, b_copy, NULL);
}

// Computes the inverse o of a modulo n, and if no inverse is
// found, o is set to 0.
//
// o: modular inverse stored here
// a: base value
// n: modulo value
void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
    // Initialize necessary mpz_t variables
    mpz_t r, r_prime, t, t_prime, q, temp;
    mpz_inits(r, r_prime, t, t_prime, q, temp, NULL);

    // r = n, r_prime = a, t = 0, t_prime = 1
    mpz_set(r, n);
    mpz_set(r_prime, a);
    mpz_set_ui(t, 0);
    mpz_set_ui(t_prime, 1);

    // While r_prime does not equal 0
    while (mpz_cmp_ui(r_prime, 0) != 0) {
        // q = floor divide r / r_prime
        mpz_fdiv_q(q, r, r_prime);

        // temp is used for preserving the original value of r.
        // r = r_prime
        // r_prime = r - q * r_prime
        mpz_set(temp, r);
        mpz_set(r, r_prime);
        mpz_submul(temp, q, r_prime);
        mpz_set(r_prime, temp);

        // Do the same operations for t and t_prime
        mpz_set(temp, t);
        mpz_set(t, t_prime);
        mpz_submul(temp, q, t_prime);
        mpz_set(t_prime, temp);
    }

    // If r > 1, there is no inverse
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(o, 0);
        mpz_clears(r, r_prime, t, t_prime, q, temp, NULL);
        return;
    }
    // If t < 0, add n to t
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    // Computed modular inverse stored in o
    mpz_set(o, t);
    mpz_clears(r, r_prime, t, t_prime, q, temp, NULL);
}

// Performs modular exponentiation by computing the base a raised to
// d power modulo n and stores the result in o.
//
// o: store computed result here
// a: base value
// d: raise base to this power
// n: modulo value
void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
    // v = 1, p = a, d_copy = d
    mpz_t v, p, d_copy, temp;
    mpz_inits(v, p, temp, d_copy, NULL);
    mpz_set_ui(v, 1);
    mpz_set(p, a);
    mpz_set(d_copy, d);

    // While d_copy > 0
    while (mpz_cmp_ui(d_copy, 0) > 0) {
        // If d_copy is odd
        if (mpz_odd_p(d_copy) != 0) {
            // v = (v * p) % n
            mpz_mul(v, v, p);
            mpz_mod(v, v, n);
        }
        // p = (p * p) % n
        mpz_mul(p, p, p);
        mpz_mod(p, p, n);
        // d_copy = floor divide d_copy / 2
        mpz_fdiv_q_ui(d_copy, d_copy, 2);
    }
    // Set o to computed power mod (v)
    mpz_set(o, v);
    mpz_clears(v, p, temp, d_copy, NULL);
}

// Uses the Miller-Rabin primality test to check whether or not n is prime
// using iters number of iterations.
//
// n: number that is checked
// iters: number of iterations
bool is_prime(mpz_t n, uint64_t iters) {
    // If number n is 2 or 3, it is prime
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0) {
        return true;
    }
    // If number n is less than or equal to 1 or is even, it is not prime
    if (mpz_cmp_ui(n, 1) <= 0 || (mpz_even_p(n) != 0)) {
        return false;
    }

    // Initialize mpz_t variables
    mpz_t r, s, temp, y, j, a, n_minus1, n_minus3, s_minus1;
    mpz_inits(r, s, temp, y, j, a, n_minus1, n_minus3, s_minus1, NULL);

    // Calculate n - 1 and n - 3 for later use
    mpz_sub_ui(n_minus1, n, 1);
    mpz_sub_ui(n_minus3, n, 3);

    // r = n - 1, s = 0
    mpz_set(r, n_minus1);
    mpz_set_ui(s, 0);

    // Writes n - 1 = 2^s*r such that r is odd
    while (mpz_even_p(r) != 0) {
        // r = floor divide r / 2
        mpz_fdiv_q_ui(r, r, 2);
        // s = s + 1
        mpz_add_ui(s, s, 1);
    }

    for (uint64_t i = 1; i <= iters; i += 1) {
        // Generate a random value a between 2 and n-2, store in a,
        // and calculate the pow mod of a raised to power r modulo n.
        mpz_urandomm(a, state, n_minus3);
        mpz_add_ui(a, a, 2);
        pow_mod(y, a, r, n);

        // If y != 1 and y != (n - 1)
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, n_minus1) != 0) {
            mpz_set_ui(j, 1);
            // While j < s and y != (n - 1)
            while (mpz_cmp(j, s) < 0 && mpz_cmp(y, n_minus1) != 0) {
                mpz_set_ui(temp, 2);
                // Calculate pow mod of y raised to power 2 modulo n
                pow_mod(y, y, temp, n);
                // If y == 1, the number is not prime
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(r, s, temp, y, j, a, n_minus1, n_minus3, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            // If y != (n - 1), the number is not prime
            if (mpz_cmp(y, n_minus1) != 0) {
                mpz_clears(r, s, temp, y, j, a, n_minus1, n_minus3, NULL);
                return false;
            }
        }
    }
    mpz_clears(r, s, temp, y, j, a, n_minus1, n_minus3, NULL);
    // Made it through the for loop, so the number is prime
    return true;
}

// Creates a random prime number p that is bits number
// of bits long.
//
// p: random prime number that is generated
// bits: length of p in bits
// iters: used in is_prime() call
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    // Offset used for range of bits
    mpz_t offset, x;
    mpz_inits(offset, x, NULL);
    mpz_set_ui(x, 1);
    mpz_mul_2exp(offset, x, bits - 1);
    do {
        // Generate a random number that is bits number of bits
        mpz_urandomb(p, state, bits - 1);
        mpz_add(p, p, offset);
    } while (!is_prime(p, iters)); // Until that number is prime
    mpz_clears(offset, x, NULL);
}
