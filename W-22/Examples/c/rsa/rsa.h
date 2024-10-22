#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <gmp.h>

//
// Miller-Rabin probabilistic primality test (Cormen).
//
// n: an initialized mpz_t to perform primality test of.
// iters: number of Miller-Rabin iterations to perform.
//
bool isPrime(mpz_t n, uint64_t iters);

//
// Miller-Rabin probabilistic primality test (Wikipedia).
//
// n: an initialized mpz_t to perform primality test of.
// iters: number of Miller-Rabin iterations to perform.
//
bool isPrimeWiki(mpz_t n, uint64_t iters);

//
// Miller-Rabin probabilistic primality test (book).
//
// n: an initialized mpz_t to perform primality test of.
// iters: number of Miller-Rabin iterations to perform.
//
bool isPrimeBook(mpz_t n, uint64_t iters);

//
// Generates a prime of a specified minimum number of bits.
// The primality is tested using Miller-Rabin.
//
// p: a pre-initialized mpz_t that will hold the created prime.
// bits: minimum number of bits the created prime will have.
// iters: number of Miller-Rabin iterations to perform.
//
uint64_t rsaMakePrime(mpz_t p, uint64_t bits, uint64_t iters);

//
// Initializes the random state needed for all RSA operations.
// Must be called before any RSA operations are used.
//
// seed: the seed to seed the random state with.
//
void rsaInitialize(uint64_t seed);

//
// Frees any memory used by the random state used for RSA operations.
// Must be called after all RSA operations are used.
//
void rsaFinalize(void);

//
// Generates the components for a new public RSA key.
// p and q will be large primes with n their product.
// The product n will be of a specified minimum number of bits.
// The primality is tested using Miller-Rabin.
// The public exponent e will have around the same number of bits as n.
// All mpz_t arguments are expected to be initialized.
//
// p: will store the first large prime.
// q: will store the second large prime.
// n: will store the product of p and q.
// e: will store the public exponent.
//
void rsaMakePublic(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters);

//
// Writes a public RSA key to a file.
// Public key contents: n, e, signature, username.
// All mpz_t arguments are expected to be initialized.
//
// n: the public modulus.
// e: the public exponent.
// s: the signature of the username.
// username: the username that was signed as s.
//
void rsaWritePublic(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile);

//
// Reads a public RSA key from a file.
// Public key contents: n, e, signature, username.
// All mpz_t arguments are expected to be initialized.
//
// n: will store the public modulus.
// e: will store the public exponent.
// s: will store the signature.
// username: an allocated array to hold the username.
// pbfile: the file containing the public key
//
void rsaReadPublic(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile);

//
// Generates the components for a new private RSA key.
// Requires an accompanying RSA public key to complete the pair.
// All mpz_t arguments are expected to be initialized.
//
// d: will store the RSA private key.
// e: the precomputed public exponent.
// p: the first large prime from the public key generation.
// p: the second large prime from the public key generation.
//
void rsaMakePrivate(mpz_t d, mpz_t e, mpz_t p, mpz_t q);

//
// Writes a private RSA key to a file.
// Private key contents: n, d.
// All mpz_t arguments are expected to be initialized.
//
// n: the public modulus.
// d: the private key.
// pvfile: the file to write the private key to.
//
void rsaWritePrivate(mpz_t n, mpz_t d, FILE *pvfile);

//
// Reads a private RSA key from a file.
// Private key contents: n, d.
// All mpz_t arguments are expected to be initialized.
//
// n: will store the public modulus.
// d: will store the private key.
void rsaReadPrivate(mpz_t n, mpz_t d, FILE *pvfile);

//
// Encrypts a message given an RSA public exponent and modulus.
// All mpz_t arguments are expected to be initialized.
//
// c: will store the encrypted message.
// m: the message to encrypt.
// e: the public exponent.
// n: the public modulus.
//
void rsaEncrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n);

//
// Encrypts an entire file given an RSA public modulus and exponent.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to encrypt.
// outfile: the output file to write the encrypted input to.
// n: the public modulus.
// e: the public exponent.
//
void rsaEncryptFile(FILE *infile, FILE *outfile, mpz_t n, mpz_t e);

//
// Decrypts some ciphertext given an RSA private key and public modulus.
// All mpz_t arguments are expected to be initialized.
//
// m: will store the decrypted message.
// c: the ciphertext to decrypt.
// d: the private key.
// n: the public modulus.
//
void rsaDecrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n);

//
// Decrypts an entire file given an RSA public modulus and private key.
// All mpz_t arguments are expected to be initialized.
// All FILE * arguments are expected to be properly opened.
//
// infile: the input file to decrypt.
// outfile: the output file to write the decrypted input to.
// n: the public modulus.
// d: the private key.
//
void rsaDecryptFile(FILE *infile, FILE *outfile, mpz_t n, mpz_t d);

//
// Signs some message given an RSA private key and public modulus.
// All mpz_t arguments are expected to be initialized.
//
// s: will store the signed message (the signature).
// m: the message to sign.
// d: the private key.
// n: the public modulus.
//
void rsaSign(mpz_t s, mpz_t m, mpz_t d, mpz_t n);

//
// Verifies some signature given an RSA public exponent and modulus.
// Requires the expected message for verification.
// All mpz_t arguments are expected to be initialized.
//
// m: the expected message.
// s: the signature to verify.
// e: the public exponent.
// n: the public modulus.
// returns: true if signature is verified, false otherwise.
//
bool rsaVerify(mpz_t m, mpz_t s, mpz_t e, mpz_t n);
