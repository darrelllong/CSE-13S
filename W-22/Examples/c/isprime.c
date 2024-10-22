#include "isprime.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <sys/random.h>
#else
#include <unistd.h>
#endif

static inline bool even(uint64_t n) { return (n & 0x1) == 0; }

static inline bool  odd(uint64_t n) { return (n & 0x1) == 1; }

//
// return a**d (mod n) using the method of repeated squares
//
static inline uint64_t powerMod(uint64_t a, uint64_t d, uint64_t n) {
    uint64_t v = 1;
    for (uint64_t p = a; d > 0; d >>= 1) {
        if (odd(d)) {
            v = (v * p) % n;
        }
        p = (p * p) % n;
    }
    return v;
}

//
// return 64 bits of randomness
//
static inline uint64_t uniform(void) {
    return ((uint64_t) random() << 32) | (uint64_t) random();
}

//
// Determine for certain if the number is composite, but only perhaps
// if it is prime. Each time it has at most a ¼ chance of being wrong.
//
static inline bool witness(uint64_t a, uint64_t n) {
    //
    // Factor n into u * 2**t + 1
    //
    uint64_t u = n - 1, t = 0;
    while (even(u)) {
        t += 1;
        u /= 2;
    }
    uint64_t x = powerMod(a, u, n);
    for (uint64_t i = 0; i < t; i += 1) {
        uint64_t y = powerMod(x, 2, n);
        if (y == 1 && x != 1 && x != n - 1) {
            return true;
        }
        x = y;
    }
    return x != 1;
}

//
// Miller-Rabin probabilistic primality test
//
bool isPrime(uint64_t n, uint64_t k) {
    if (n < 2 || (n != 2 && n % 2 == 0)) { // 0, 1 are not prime, nor are even numbers (except 2)
        return false;
    }
    if (n < 4) { // 2, 3 are prime
        return true;
    }
    //
    // We test all others
    //
    for (uint64_t j = 0; j < k; j += 1) {
        uint64_t a = (uniform() % (n - 1)) + 1; // a ∊ [1, ..., n - 1]
        if (witness(a, n)) {
            return false;
        }
    }
    return true;
}

void primeInitialize(void) {
    uint32_t b;
    if (getentropy(&b, 4)) {
        fprintf(stderr, "Fail: no entropy!\n");
        exit(1);
    }
    srandom(b);
    return;
}

void primeFinalize(void) {
    return;
}
