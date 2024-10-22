#include <stdio.h>
#include <time.h>
#include <gmp.h>

// Computes the greatest common divisor of `a` and `b`.
// The value of the computed divisor is stored in `d`.
void gcd(mpz_t d, mpz_t a, mpz_t b) {
    mpz_t x, y, t;
    mpz_inits(x, y, t, NULL);
    mpz_set(x, a); // x = a
    mpz_set(y, b); // y = b

    while (mpz_cmp_ui(y, 0) != 0) { // while y != 0:
        mpz_set(t, y);              //     t = y
        mpz_mod(y, x, y);           //     y = x % y
        mpz_set(x, t);              //     x = t
    }

    mpz_set(d, x); // d = x
    mpz_clears(x, y, t, NULL); // Free local mpz_t variables.
    return;
}

int main(void) {
    gmp_randstate_t state;              // Random state.
    gmp_randinit_mt(state);             // Use Mersenne Twister.
    gmp_randseed_ui(state, time(NULL)); // Seed the state with current time.

    mpz_t d, a, b;
    mpz_inits(d, a, b, NULL); // Initialize local mpz_t variables.

    // Set `a` and `b` as two random 128-bit integers, then compute gcd(a, b).
    mpz_urandomb(a, state, 128);
    mpz_urandomb(b, state, 128);
    gcd(d, a, b);

    // Print out variables using GMP printf formatting.
    gmp_printf("a = %Zd\n", a);
    gmp_printf("b = %Zd\n", b);
    gmp_printf("gcd(a, b) = %Zd\n", d);

    mpz_clears(d, a, b, NULL); // Clear local mpz_t variables.
    return 0;
}
