#include "randstate.h"
#include "rsa.h"

#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#define OPTIONS "b:i:n:d:s:vh"

static uint64_t nbits = 256;
static uint64_t iters = 50;
static uint64_t seed;

void print_usage(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Generates an RSA public/private key pair.\n\n"
                    "USAGE\n"
                    "   ./keygen [-hv] [-b bits] [-i iters] [-n pbfile] [-d pvfile]\n\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -b bits         Minimum bits needed for public key n.\n"
                    "   -i iters        Miller-Rabin iterations for testing primes (default: 50).\n"
                    "   -n pbfile       Public key file (default: rsa.pub).\n"
                    "   -d pvfile       Private key file (default: rsa.priv).\n"
                    "   -s seed         Random seed for testing.\n");
    exit(0);
}

void print_verbose(char *username, mpz_t s, mpz_t p, mpz_t q, mpz_t n, mpz_t e, mpz_t d) {
    fprintf(stderr, "user = %s\n", username);
    gmp_printf("s (%lu bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
    gmp_printf("p (%lu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
    gmp_printf("q (%lu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
    gmp_printf("n (%lu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    gmp_printf("e (%lu bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
    gmp_printf("d (%lu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
}

int main(int argc, char **argv) {
    int opt = 0;
    seed = time(NULL);
    bool verbose = false;
    bool help = false;
    FILE *pbfile;
    FILE *pvfile;
    char *pub = "rsa.pub";
    char *priv = "rsa.priv";

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': nbits = atoi(optarg); break;
        case 'i': iters = atoi(optarg); break;
        case 'n': pub = optarg; break;
        case 'd': priv = optarg; break;
        case 's': seed = atoi(optarg); break;
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        case '?': help = true; break;
        default: help = true; break;
        }
    }

    // Open public and private key files, set file permissions for private key file
    pbfile = fopen(pub, "w");
    pvfile = fopen(priv, "w");
    fchmod(fileno(pvfile), 0600);

    // If help option is enabled, print the usage guide
    if (help) {
        print_usage();
    }

    // Initialize the random state with seed
    randstate_init(seed);

    // Create a public and a private key
    // p and q: large prime numbers
    // n: product of p and q
    // e: public exponent
    // d: private key
    mpz_t p, q, n, e, d;
    mpz_inits(p, q, n, e, d, NULL);
    rsa_make_pub(p, q, n, e, nbits, iters);
    rsa_make_priv(d, e, p, q);

    // Get the current user's name as a string
    char *username = getenv("USER");
    // Convert the username to mpz_t and then compute the signature
    // s: signature
    // d: private key
    // n: public modulus
    mpz_t user, s;
    mpz_inits(user, s, NULL);
    mpz_set_str(user, username, 62);
    rsa_sign(s, user, d, n);

    // Write the public and private key to respective file
    rsa_write_pub(n, e, s, username, pbfile);
    rsa_write_priv(n, d, pvfile);

    // If verbose option is enabled, print verbose output
    if (verbose) {
        print_verbose(username, s, p, q, n, e, d);
    }

    // Close the public and private key files
    fclose(pbfile);
    fclose(pvfile);

    // Clear the random state
    randstate_clear();

    // Clear mpz_t variables
    mpz_clears(p, q, n, e, d, user, s, NULL);
}
