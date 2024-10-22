#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <gmp.h>

void gcd(mpz_t g, mpz_t a, mpz_t b);

void lcm(mpz_t l, mpz_t a, mpz_t b);

void mod_inverse(mpz_t o, mpz_t a, mpz_t n);

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n);

//
// Miller-Rabin probabilistic primality test (Cormen).
//
// n: an initialized mpz_t to perform primality test of.
// iters: number of Miller-Rabin iterations to perform.
//
bool is_prime(mpz_t n, uint64_t iters);

//
// Generates a prime of a specified minimum number of bits.
// The primality is tested using Miller-Rabin.
//
// p: a pre-initialized mpz_t that will hold the created prime.
// bits: minimum number of bits the created prime will have.
// iters: number of Miller-Rabin iterations to perform.
//
void make_prime(mpz_t p, uint64_t bits, uint64_t iters);
