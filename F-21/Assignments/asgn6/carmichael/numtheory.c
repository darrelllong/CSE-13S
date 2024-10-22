#include "numtheory.h"
#include "randstate.h"

void gcd(mpz_t o, mpz_t a, mpz_t b) {
    mpz_t t, tA, tB;
    mpz_init(t);
    mpz_init_set(tA, a);
    mpz_init_set(tB, b);

    while (mpz_cmp_ui(tB, 0) != 0) {
        mpz_set(t, tB);
        mpz_mod(tB, tA, tB);
        mpz_set(tA, t);
    }

    mpz_set(o, tA);
    mpz_clears(t, tA, tB, NULL);
}

void lcm(mpz_t l, mpz_t a, mpz_t b) {
    mpz_t x, y;
    mpz_inits(x, y, NULL);

    mpz_mul(x, a, b);      // x = ab
    gcd(y, a, b);          // y = gcd(a, b)
    mpz_divexact(l, x, y); // lcm(a, b) = ab / gcd(a, b)

    mpz_clears(x, y, NULL);
}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
    mpz_t t, nT, r, nR, q, tmpA, tmpB;
    mpz_inits(t, nT, r, nR, q, tmpA, tmpB, NULL);

    mpz_set_ui(t, 0);   // t = 0
    mpz_set_ui(nT, 1);  // nT = 1
    mpz_set(r, n);      // r = n
    mpz_set(nR, a);     // nR = a

    while (mpz_cmp_ui(nR, 0) != 0) {
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
    mpz_clears(t, nT, r, nR, q, tmpA, tmpB, NULL);
}

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
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
}

bool is_prime(mpz_t n, uint64_t k) {
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

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mpz_t temp, mod, offset;
    mpz_inits(temp, mod, offset, NULL);
    mpz_set_ui(temp, 1);
    mpz_mul_2exp(offset, temp, bits); // offset = 2^{bits}

    do {
        do {
            mpz_urandomb(p, state, bits); // Guess a (random() % 2^{bits - 1})
        } while (mpz_even_p(p));          // But skip even numbers
        mpz_add(p, p, offset);            // And add 2^{bits} to avoid small numbers
    } while (!is_prime(p, iters)); // Until we find a prime

    mpz_clears(temp, mod, offset, NULL); // Free local variables
}
