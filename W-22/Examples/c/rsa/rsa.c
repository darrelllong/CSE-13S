#include <inttypes.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include "rsa.h"

#ifdef __APPLE__
#include <sys/random.h>
#else
#include <unistd.h>
#endif

static gmp_randstate_t state; // Randomness state information

static void powerMod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
    mpz_t v, p, t;
    mpz_init_set_ui(v, 1); // v = 1
    mpz_init_set(t, d);    // t = d
    mpz_init_set(p, a);    // p = a
    while (mpz_cmp_ui(t, 0)) {
        if (mpz_odd_p(t)) {
            mpz_mul(v, v, p); // v = v * p
            mpz_mod(v, v, n); // v = v % n
        }
        mpz_mul(p, p, p); // p = p * p
        mpz_mod(p, p, n); // p = p % n
        mpz_tdiv_q_2exp(t, t, 1); // t = t / 2
    }
    mpz_set(o, v); // Set return value
    mpz_clears(v, p, t, NULL); // Free local variables
    return;
}

static bool witness(mpz_t a, mpz_t n) {
    mpz_t u, t, x, y, two, m;
    mpz_inits(u, t, x, y, two, m, NULL); // Initialize local variables

                         // Factor n into u * 2**t + 1
    mpz_set_ui(t, 0);    // t = 0
    mpz_sub_ui(u, n, 1); // u = n - 1
    while (mpz_even_p(u)) {
        mpz_add_ui(t, t, 1);      // t += 1
        mpz_tdiv_q_2exp(u, u, 1); // u /= 2
    }

    mpz_set_ui(two, 2);  // Constant for powerMod()
    mpz_sub_ui(m, n, 1); // m = n - 1

    powerMod(x, a, u, n);
    for (uint64_t i = 1; mpz_cmp_ui(t, i) >= 0; i += 1) {
        powerMod(y, x, two, n);
        if (mpz_cmp_ui(y, 1) == 0 && mpz_cmp_ui(x, 1) != 0 && mpz_cmp(x, m) != 0) {
            mpz_clears(u, t, x, y, two, m, NULL); // Free local variables
            return true;
        }
        mpz_set(x, y); // Update x
    }

    if (mpz_cmp_ui(x, 1) != 0) {
        mpz_clears(u, t, x, y, two, m, NULL); // Free local variables
        return true;
    }

    mpz_clears(u, t, x, y, two, m, NULL); // Free local variables
    return false;
}

// Miller-Rabin probabilistic primality test
bool isPrime(mpz_t n, uint64_t k) {
    if (mpz_cmp_ui(n, 2) < 0) { // 0, 1 are not prime
        return false;
    }
    if (mpz_cmp_ui(n, 4) < 0) { // 2, 3 are prime
        return true;
    }

    // We test all others
    mpz_t a, m;
    mpz_inits(a, m, NULL);
    mpz_sub_ui(m, n, 1); // m = n - 1

    for (uint64_t j = 0; j < k; j += 1) {
        mpz_urandomm(a, state, m); // a ∊ [0, n - 2]
        mpz_add_ui(a, a, 1);       // Shift to a ∊ [1, n - 1]
        if (witness(a, n)) {
            mpz_clears(a, m, NULL); // Free local variables
            return false;
        }
    }

    mpz_clears(a, m, NULL); // Free local variables
    return true;
}

// Written following Wikipedia pseudocode.
// https://en.wikipedia.org/wiki/Miller–Rabin_primality_test
bool isPrimeWiki(mpz_t n, uint64_t k) {
    if (mpz_cmp_ui(n, 2) < 0) {
        return false; // 0 and 1 aren't prime
    }
    if (mpz_cmp_ui(n, 4) < 0) {
        return true; // 2 and 3 are prime
    }
    if (mpz_even_p(n)) {
        return false; // Can't be prime if even
    }

    mpz_t a, m, r, d, x, t, j;
    mpz_inits(a, m, r, d, x, t, j, NULL);

    // Factor powers of 2 from n - 1
    // We want n = 2**r * d + 1
    mpz_set_ui(r, 0); // r = 0
    mpz_sub_ui(d, n, 1); // d = n - 1
    while (mpz_even_p(d)) {
        mpz_add_ui(r, r, 1); // r += 1
        mpz_tdiv_q_2exp(d, d, 1); // d /= 2
    }

    mpz_sub_ui(m, n, 1); // m = n - 1
    mpz_sub_ui(t, n, 3); // t = n - 3
    bool loop_witness = false; // Because gotos are harmful

    for (uint64_t i = 0; i < k; i += 1) {
        // Pick a random integer a in range [2, n - 2]
        mpz_urandomm(a, state, t); // a ∊ [0, n - 4]
        mpz_add_ui(a, a, 2); // a ∊ [2, n - 2]
        mpz_powm(x, a, d, n); // x = a**d mod n

        if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, m) == 0) {
            continue; // Continue if x == 1 or x == n - 1 == m
        }

        // Start at 1 to repeat r - 1 times
        for (mpz_set_ui(j, 1); mpz_cmp(j, r) < 0; mpz_add_ui(j, j, 1)) {
            mpz_powm_ui(x, x, 2, n);
            if (mpz_cmp(x, m) == 0) {
                loop_witness = true;
                break; // Continue outer witness loop if x == n - 1 == m
            }
        }

        if (loop_witness) {
            loop_witness = false;
            continue;
        }

        mpz_clears(a, m, r, d, x, t, j, NULL); // Free local variables
        return false;
    }

    mpz_clears(a, m, r, d, x, t, j, NULL); // Free local variables
    return true;
}

bool isPrimeBook(mpz_t n, uint64_t k) {
    if (mpz_cmp_ui(n, 2) < 0) {
        return false; // 0 and 1 aren't prime
    }
    if (mpz_cmp_ui(n, 4) < 0) {
        return true; // 2 and 3 are prime
    }
    if (mpz_even_p(n)) {
        return false; // Can't be prime if even
    }

    // Initialize all local variables for ease of use later.
    mpz_t s, r, u, m, t, a, y, j;
    mpz_inits(s, r, u, m, t, a, y, j, NULL);

    // Step 1) Write n - 1 = r2**s such that r is odd.
    mpz_set_ui(s, 0); // s = 0
    mpz_sub_ui(r, n, 1); // r = n - 1
    while (mpz_even_p(r)) {
        mpz_add_ui(s, s, 1); // r += 1
        mpz_tdiv_q_2exp(r, r, 1); // d /= 2
    }

    mpz_sub_ui(u, s, 1); // u = s - 1 (for step 2.3)
    mpz_sub_ui(m, n, 1); // m = n - 1 (for step 2.3)
    mpz_sub_ui(t, n, 3); // t = n - 3 (for random number range)

    // Step 2) For i from 1 to t...
    for (uint64_t i = 1; i <= k; i += 1) {
        // Step 2.1) Choose a random integer a, 2 <= a <= n - 2.
        mpz_urandomm(a, state, t); // a ∊ [0, n - 4]
        mpz_add_ui(a, a, 2); // a ∊ [2, n - 2]

        // Step 2.2) Compute y = a**r mod n.
        mpz_powm(y, a, r, n);

        // Step 2.3) If y != 1 and y != n - 1...
        if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, m) != 0) {
            // Loop while j <= s - 1 and y != n - 1.
            for (mpz_set_ui(j, 1); mpz_cmp(j, u) <= 0 && mpz_cmp(y, m) != 0; mpz_add_ui(j, j, 1)) {
                // Compute y = y**2 mod n.
                mpz_powm_ui(y, y, 2, n);

                // If y == 1, it's composite.
                if (mpz_cmp_ui(y, 1) == 0) {
                    mpz_clears(s, r, m, u, t, y, j, NULL);
                    return false;
                }
            }

            // If y != n - 1, it's composite.
            if (mpz_cmp(y, m) != 0) {
                mpz_clears(s, r, u, m, t, a, y, j, NULL);
                return false;
            }
        }
    }

    // Step 3) It's prime.
    mpz_clears(s, r, u, m, t, a, y, j, NULL);
    return true;
}

void rsaInitialize(uint64_t seed) {
    uint32_t b;
    if (getentropy(&b, 4)) {
        fprintf(stderr, "Fail: no entropy!\n");
        exit(1);
    }
    srandom(b);
    gmp_randinit_mt(state); // Mersenne Twister
    gmp_randseed_ui(state, seed);
    return;
}

void rsaFinalize(void) {
    gmp_randclear(state);
    return;
}

uint64_t rsaMakePrime(mpz_t p, uint64_t bits, uint64_t confidence) {
    uint64_t count = 0;

    mpz_t temp, mod, offset;
    mpz_inits(temp, mod, offset, NULL);
    mpz_set_ui(temp, 1);
    mpz_mul_2exp(offset, temp, bits); // offset = 2^{bits}

    do {
        do {
            mpz_urandomb(p, state, bits); // Guess a (random() % 2^{bits - 1})
        } while (mpz_even_p(p));          // But skip even numbers
        mpz_add(p, p, offset);            // And add 2^{bits} to avoid small numbers
        count += 1;
    } while (!isPrimeBook(p, confidence)); // Until we find a prime

    mpz_clears(temp, mod, offset, NULL); // Free local variables
    return count;
}

static void rsaInverse(mpz_t o, mpz_t a, mpz_t n) {
    mpz_t t, nT, r, nR, q;
    mpz_init_set_ui(t, 0);  // t = 0
    mpz_init_set_ui(nT, 1); // nT = 1
    mpz_init_set(r, n);     // r = n
    mpz_init_set(nR, a);    // nR = a
    mpz_init(q);

    mpz_t tmpA, tmpB; // Two temporaries to mimic parallel assignment
    mpz_inits(tmpA, tmpB, NULL);

    while (mpz_cmp_ui(nR, 0)) {
        mpz_tdiv_q(q, r, nR);

        mpz_set(tmpA, nT); // Faking parallel assignment
        mpz_mul(tmpB, q, nT);
        mpz_sub(tmpB, t, tmpB);
        mpz_set(t, tmpA);
        mpz_set(nT, tmpB);

        mpz_set(tmpA, nR); // Faking parallel assignment
        mpz_mul(tmpB, q, nR);
        mpz_sub(tmpB, r, tmpB);
        mpz_set(r, tmpA);
        mpz_set(nR, tmpB);
    }
    if (mpz_cmp_ui(r, 1) > 0) { // Zero means there is no inverse
        mpz_set_ui(t, 0);
    }
    if (mpz_cmp_ui(t, 0) < 0) { // Inverses should be positive
        mpz_add(t, t, n);
    }

    mpz_set(o, t);
    mpz_clears(t, nT, r, nR, q, tmpA, tmpB, NULL); // Free local variables
    return;
}

static void gcd(mpz_t o, mpz_t a, mpz_t b) {
    mpz_t t, tA, tB;
    mpz_init(t);
    mpz_init_set(tA, a);
    mpz_init_set(tB, b);
    while (mpz_cmp_ui(tB, 0)) {
        mpz_set(t, tB);
        mpz_mod(tB, tA, tB);
        mpz_set(tA, t);
    }
    mpz_set(o, tA);
    mpz_clears(t, tA, tB, NULL); // Free local variables
    return;
}

void rsaMakePublic(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t confidence) {
    uint64_t pbits = (random() % (nbits / 2)) + (nbits / 4); // pbits ∊ [n/4, ..., 3n/4]
    uint64_t qbits = nbits - pbits;

    rsaMakePrime(p, pbits, confidence); // Choose our private primes p & q
    rsaMakePrime(q, qbits, confidence);

    mpz_t d, totient, tP, tQ, tE;
    mpz_inits(d, totient, tP, tQ, tE, NULL);
    mpz_sub_ui(tP, p, 1);
    mpz_sub_ui(tQ, q, 1);
    mpz_mul(n, p, q);         // n = p × q
    mpz_mul(totient, tP, tQ); // 𝜑(n) = (p - 1)(q - 1)

    do {
        mpz_urandomb(tE, state, nbits);
        gcd(d, tE, totient);
    } while (mpz_cmp_ui(d, 1)); // gcd(tE, 𝜑(n)) = 1

    mpz_set(e, tE); // We have found our e
    mpz_clears(d, totient, tP, tQ, tE, NULL); // Free local variables
    return;
}

void rsaReadPublic(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    fscanf(pbfile, "%s\n", username);
    return;
}

void rsaWritePublic(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile) {
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    fprintf(pbfile, "%s\n", username);
    return;
}

void rsaMakePrivate(mpz_t d, mpz_t e, mpz_t p, mpz_t q) {
    mpz_t totient, tP, tQ;
    mpz_inits(totient, tP, tQ, NULL);
    mpz_sub_ui(tP, p, 1);
    mpz_sub_ui(tQ, q, 1);
    mpz_mul(totient, tP, tQ);  // 𝜑(n) = (p - 1)(q - 1)
    rsaInverse(d, e, totient); // e * d ≡ 1 (mod 𝜑(n))
    mpz_clears(totient, tP, tQ, NULL);
    return;
}

void rsaWritePrivate(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
    return;
}

void rsaReadPrivate(mpz_t n, mpz_t d, FILE *pvfile) {
    gmp_fscanf(pvfile, "%Zx\n", n);
    gmp_fscanf(pvfile, "%Zx\n", d);
    return;
}

void rsaEncrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n) {
    powerMod(c, m, e, n); // c = m**e (mod n)
    return;
}

void rsaEncryptFile(FILE *infile, FILE *outfile, mpz_t n, mpz_t e) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);

    // Encrypt input in small blocks, where the block is always prepended with 0xFF.
    // This is due to the possibility of having blocks containing all zeroes.
    // Having the 0xFF at the beginning ensures proper mpz import/export.
    size_t nbytes = 0;
    size_t blocksize = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *block = calloc(blocksize, sizeof(uint8_t));
    if (block) { block[0] = 0xFF; } // 0th byte is the prepended 0xFF.

    while (!feof(infile)) {
        // Read into block buffer after 0th byte, since the 0th byte is the 0xFF.
        // The original block size includes the 0xFF, so we read one less.
        if ((nbytes = fread(block + 1, sizeof(uint8_t), blocksize - 1, infile)) > 0) {
            // Convert bytes read + 0xFF byte into message to encrypt.
            mpz_import(m, nbytes + 1, 1, sizeof(uint8_t), 0, 0, block);
            rsaEncrypt(c, m, e, n);
            gmp_fprintf(outfile, "%Zx\n", c);
        }
    }

    mpz_clears(m, c, NULL);
    free(block);
    return;
}

void rsaDecryptFile(FILE *infile, FILE *outfile, mpz_t n, mpz_t d) {
    mpz_t m, c;
    mpz_inits(m, c, NULL);

    // Decrypt input in small blocks.
    // Note: the encrypt program always prepends blocks with 0xFF.
    // This is due to the possibility of having blocks containing all zeroes.
    // Having the 0xFF at the beginning ensures proper mpz import/export.
    size_t nbytes = 0;
    size_t blocksize = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *block = calloc(blocksize, sizeof(uint8_t));

    while (!feof(infile)) {
        if (gmp_fscanf(infile, "%Zx\n", c) > 0) {
            rsaDecrypt(m, c, d, n);
            // Convert decrypted message back into bytes.
            mpz_export(block, &nbytes, 1, sizeof(uint8_t), 0, 0, m);

            // Write after 0th byte, since that must be the prepended 0xFF.
            // This means we also write one less than the number of bytes that were exported.
            fwrite(block + 1, sizeof(uint8_t), nbytes - 1, outfile);
        }
    }

    mpz_clears(m, c, NULL);
    free(block);
    return;
}

void rsaDecrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n) {
    powerMod(m, c, d, n); // m = c**d (mod n)
    return;
}

void rsaSign(mpz_t s, mpz_t m, mpz_t d, mpz_t n) {
    powerMod(s, m, d, n); // s = m**d (mod n)
    return;
}

bool rsaVerify(mpz_t m, mpz_t s, mpz_t e, mpz_t n) {
    mpz_t tmpM;
    mpz_init(tmpM); // Temporary to avoid modifying original signature.
    powerMod(tmpM, s, e, n); // m = s**e (mod n) if the signature is valid
    bool verified = mpz_cmp(m, tmpM) == 0; // Need to save result to clear temporary.
    mpz_clear(tmpM);
    return verified;
}
