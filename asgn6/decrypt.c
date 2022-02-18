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
                    "   Decrypts data using RSA encryption.\n"
                    "   Encrypted data is encrypted by the encrypt program.\n\n"
                    "USAGE\n"
                    "   ./decrypt [-hv] [-i infile] [-o outfile] -n privkey\n\n"
                    "OPTIONS\n"
                    "   -h              Display program help and usage.\n"
                    "   -v              Display verbose program output.\n"
                    "   -i infile       Input file of data to decrypt (default: stdin).\n"
                    "   -o outfile      Output file for decrypted data (default: stdout).\n"
                    "   -n pvfile       Private key file (default: rsa.priv).\n");
    exit(0);
}

void print_verbose(mpz_t n, mpz_t d) {
    gmp_printf("n (%lu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    gmp_printf("d (%lu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
}

int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    bool help = false;
    bool i = false;
    bool o = false;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    FILE *pvfile;
    char *in = "";
    char *out = "";
    char *priv = "rsa.priv";

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
        case 'n': priv = optarg; break;
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

    // Open private key file for reading
    pvfile = fopen(priv, "r");

    // Initialize variables and read from private key file
    // n: public modulus
    // d: private key
    mpz_t n, d;
    mpz_inits(n, d, NULL);
    rsa_read_priv(n, d, pvfile);

    // If verbose option is enabled, print verbose output
    if (verbose) {
        print_verbose(n, d);
    }

    // Decrypt the file
    rsa_decrypt_file(infile, outfile, n, d);

    // Close the private key file and clear mpz_t variables
    fclose(pvfile);
    mpz_clears(n, d, NULL);
}
