#include "randstate.h"
#include "rsa.h"

#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/stat.h>

#define OPTIONS "i:o:n:vh"

void print_usage(void) {
    fprintf(stderr, "SYNOPSIS\n"
                    "   Encrypts data using RSA encryption.\n"
                    "   Encrypted data is decrypted by the decrypt program.\n\n"
                    "USAGE\n"
                    "   ./encrypt [-hv] [-i infile] [-o outfile] [-n pubkey]\n\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -i infile       Input file of data to encrypt (default: stdin).\n"
                    "   -o outfile      Output file for encrypted data (default: stdout).\n"
                    "   -n pbfile       Public key file (default: rsa.pub).\n");
    exit(0);
}

void print_verbose(char *username, mpz_t s, mpz_t n, mpz_t e) {
    fprintf(stderr, "user = %s\n", username);
    gmp_printf("s (%lu bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
    gmp_printf("n (%lu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    gmp_printf("e (%lu bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
}

int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    bool help = false;
    bool i = false;
    bool o = false;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pbfile;
    char *in = "";
    char *out = "";
    char *pub = "rsa.pub";

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            i = true;
            in = optarg;
            break;
        case 'o':
            o = true;
            out = optarg;
            break;
        case 'n': pub = optarg; break;
        case 'v': verbose = true; break;
        case 'h': help = true; break;
        default: help = true; break;
        }
    }
    // If help option is enabled, print the usage guide
    if (help) {
        print_usage();
    }

    // If input or output file is specified
    if (i) {
        infile = fopen(in, "r");
    }
    if (o) {
        outfile = fopen(out, "w");
    }

    // Open the public key file to read
    pbfile = fopen(pub, "r");

    // Initialize variables and read from public key file
    // n: public modulus
    // e: public exponent
    // s: signature
    // buf: username buffer
    mpz_t n, e, s;
    mpz_inits(n, e, s, NULL);
    char buf[1024];
    rsa_read_pub(n, e, s, buf, pbfile);

    // If verbose option is enabled, print verbose output
    if (verbose) {
        print_verbose(buf, s, n, e);
    }

    // Convert username to mpz_t and verify signature
    mpz_t user;
    mpz_init_set_str(user, buf, 62);
    if (!rsa_verify(user, s, e, n)) {
        fprintf(stderr, "Error: signature couldn't be verified.\n");
        exit(0);
    }

    // Encrypt the file
    rsa_encrypt_file(infile, outfile, n, e);

    // Close the public key file and clear mpz_t variables
    fclose(pbfile);
    mpz_clears(n, e, s, user, NULL);
}
