#include <stdio.h>
#include "rsa.h"

#define TESTS 50
#define SEED  5000

int main(void) {
    mpz_t i;
    rsaInitialize(SEED);

    for (mpz_init_set_ui(i, 0); mpz_cmp_ui(i, 1 << 20) < 0; mpz_add_ui(i, i, 1)) {
        // The double initialize to make sure we get same random numbers each run.
        int clrsprime = isPrime(i, TESTS) ? 1 : 0;
        int wikiprime = isPrimeWiki(i, TESTS) ? 1 : 0;
        int bookprime = isPrimeBook(i, TESTS) ? 1 : 0;
        int mpzprime = mpz_probab_prime_p(i, TESTS) > 0 ? 1 : 0;

        // If the sum isn't either 0 or 4, there's disagreement.
        int sum = clrsprime + wikiprime + bookprime + mpzprime;
        if (sum != 0 && sum != 4) {
            gmp_printf("%Zu\n", i);
            printf(" - cormen: %s\n", clrsprime ? "true" : "false");
            printf(" - wiki: %s\n", wikiprime ? "true" : "false");
            printf(" - book: %s\n", bookprime ? "true" : "false");
            printf(" - mpz: %s\n", mpzprime ? "true" : "false");
        }
    }

    rsaFinalize();
    return 0;
}
