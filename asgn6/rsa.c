#include "rsa.h"
#include "randstate.h"
#include "numtheory.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

gmp_randstate_t state;

// Credit to Prof. Long for this function
// Computes the log base 2 of an mpz_t n.
uint32_t log_base2(mpz_t n) {
    uint32_t k = 0;
    // Uses a copy of n so as to not change the value of n
    mpz_t n_copy;
    mpz_init(n_copy);
    mpz_set(n_copy, n);
    mpz_abs(n_copy, n_copy);
    // While n_copy is greater than 0, floor divide n by 2 and increment k by 1
    while (mpz_cmp_ui(n_copy, 0) > 0) {
        mpz_fdiv_q_ui(n_copy, n_copy, 2);
        k += 1;
    }
    // Clear the mpz variable and return the computed log
    mpz_clear(n_copy);
    return k;
}

// Makes an RSA public key by creating random large primes p and q,
// their product n, and the public exponent e by finding a random
// number that is coprime to n and also bits long.
//
// p and q: two large prime numbers
// n: product of p and q
// e: public exponent
// bits: length of random numbers generated
// iters: number of Miller-Rabin iterations
void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t bits, uint64_t iters) {
    // Initialize mpz_t variables
    mpz_t rand, divisor, p_minus1, q_minus1, totient;
    mpz_inits(rand, divisor, p_minus1, q_minus1, totient, NULL);

    // Set a pseudorandom seed. Generate a random number
    // with random number of bit between (bits/4) and
    // (3*bits/4) for p. Rest of bits go to q.
    srandom(time(NULL));
    uint64_t p_bits = (random() % (bits / 2) + 1) + bits / 4;
    uint64_t q_bits = bits - p_bits;

    // Make p and q into large prime numbers. set n equal
    // to p * q.
    make_prime(p, p_bits + 1, iters);
    make_prime(q, q_bits + 1, iters);
    mpz_mul(n, p, q);

    // Set totient(n) = (p-1) * (q-1)
    mpz_sub_ui(p_minus1, p, 1);
    mpz_sub_ui(q_minus1, q, 1);
    mpz_mul(totient, p_minus1, q_minus1);

    // Loop until random number is coprime with totient n.
    do {
        mpz_urandomb(rand, state, bits);
        gcd(divisor, totient, rand);
    } while (mpz_cmp_ui(divisor, 1) != 0);
    // Set e equal to the coprime number
    mpz_set(e, rand);
    mpz_clears(rand, divisor, p_minus1, q_minus1, totient, NULL);
}

// Writes a public RSA key to pbfile, with n, e, and s written
// as hexstrings.
//
// n: value of p * q
// e: public exponent
// s: signature
// username: user's name
// pbfile: write to this file
void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n%Zx\n%Zx\n", n, e, s);
    fprintf(pbfile, "%s\n", username);
}

// Reads a public RSA key from pbfile, with n, e, and s written
// as hexstrings.
//
// n: value of p * q
// e: public exponent
// s: signature
// username: user's name
// pbfile: read from this file
void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n%Zx\n%Zx\n", n, e, s);
    fscanf(pbfile, "%s\n", username);
}

// Makes an RSA private key from primes p and q, and public
// exponent e.
//
// d: private key
// e: public exponent
// p and q: two large prime numbers
void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    // Calculate totient = (p-1) * (q-1)
    mpz_t totient, p_minus1, q_minus1;
    mpz_inits(totient, p_minus1, q_minus1, NULL);

    mpz_sub_ui(p_minus1, p, 1);
    mpz_sub_ui(q_minus1, q, 1);
    mpz_mul(totient, p_minus1, q_minus1);

    // Find modular inverse of e mod totient, output is private key value
    mod_inverse(d, e, totient);
    mpz_clears(totient, p_minus1, q_minus1, NULL);
}

// Writes a private RSA key to pvfile, with n and d  written
// as hexstrings.
//
// n: value of p * q
// d: private key
// pvfile: write to this file
void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n%Zx\n", n, d);
}

// Reads a private RSA key from pvfile, with n, e, and s written
// as hexstrings.
//
// n: value of p * q
// d: private key
// pvfile: read from this file
void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n%Zx\n", n, d);
}

// RSA encrypts a message m into cyphertext c.
//
// c: cyphertext with encrypted message
// m: message
// e: public exponent
// n: public modulus
void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    // Performs modular exponentiation and stores result in cyphertext c
    pow_mod(c, m, e, n);
}

// RSA encrypts a file infile and prints the encrypted output to
// outfile.
//
// infile: encrypt this file
// outfile: encrypted output printed to this file
// n: public modulus
// e: public exponent
void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    // Calculate block size k.
    uint32_t k = (log_base2(n) - 1) / 8;

    // Dynamically allocate memory for an array block and
    // set zeroth byte of block to 0xFF.
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));
    block[0] = 1;

    uint32_t bytes;
    mpz_t read_bytes, m;
    mpz_inits(read_bytes, m, NULL);
    do {
        // Read at most k-1 bytes, store in block after the prepended 0xFF
        bytes = fread(&block[1], sizeof(uint8_t), k - 1, infile);
        // Convert bytes to mpz_t
        mpz_import(read_bytes, bytes + 1, 1, sizeof(uint8_t), 1, 0, block);
        // Encrypt the message
        rsa_encrypt(m, read_bytes, e, n);
        if (bytes > 0) {
            // Print the message
            gmp_fprintf(outfile, "%Zx\n", m);
        }
    } while (bytes > 0); // Until there is nothing left to read
    mpz_clears(read_bytes, m, NULL);
    free(block);
}

// RSA decrypts a cyphertext c into message m.
//
// m: message from encrypted cyphertext
// c: encrypted cyphertext
// d: private key
// n: public modulus
void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    // Performs modular exponentiation and stores result in message m
    pow_mod(m, c, d, n);
}

// RSA decrypts an encrypted file infile and writes the
// decrypted file to outfile.
//
// infile: decrypt this encrypted file
// outfile: decrypted file written here
// n: public modulus
// d: private key
void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    // Calculate block size k
    uint32_t k = (log_base2(n) - 1) / 8;

    // Dynamically allocate memory for a block array
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));

    uint64_t j;
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    do {
        // Scan hexstring and store in c
        gmp_fscanf(infile, "%Zx\n", c);
        // Decrypt the cyphertext c
        rsa_decrypt(m, c, d, n);
        // Convert mpz_t to bytes
        mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, m);
        // Write the decrypted message
        fwrite(&block[1], sizeof(uint8_t), j - 1, outfile);
    } while (!feof(infile)); // Until the end of file is reached
    mpz_clears(m, c, NULL);
    free(block);
}

// Performs RSA signing to create a signature s on message m.
//
// s: signature
// m: message being signed
// d: private key
// n: public modulus
void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    // Performs modular exponentiation and stores result in signature s
    pow_mod(s, m, d, n);
}

// RSA verification function, if the signature is verified,
// return true, otherwise return false.
//
// m: expected message
// s: signature
// e: public exponent
// n: public modulus
bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    // Variable used for message comparison
    mpz_t t;
    mpz_init(t);

    // Performs modular exponentiation and stores result in t
    pow_mod(t, s, e, n);

    // If t and m are the same, the signature is verified
    // and return true, otherwise return false
    if (mpz_cmp(t, m) == 0) {
        mpz_clear(t);
        return true;
    }
    mpz_clear(t);
    return false;
}
