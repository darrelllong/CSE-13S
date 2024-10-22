#include <stdio.h>
#include <gmp.h>

void fibonacci(mpz_t x, mpz_t n) {
    // if (n < 2) { ... }
    if (mpz_cmp_ui(n, 2) < 0) {
        mpz_set(x, n);
        return;
    }

    // Initialize mpz_t variables a, b, and i,
    mpz_t a, b, i; mpz_inits(a, b, i, NULL);
    mpz_set_ui(a, 0); // a = 0
    mpz_set_ui(b, 1); // b = 1
    mpz_set_ui(x, 0); // x = 0

    // for (i = 2, i <=2; i += 1) { ... }
    for (mpz_set_ui(i, 2); mpz_cmp(i, n) <= 0; mpz_add_ui(i, i, 1)) {
        mpz_add(x, a, b);   // x = a + b
        mpz_set(a, b);      // x = b
        mpz_set(b, x);      // b = x
    }

    // Free initialized mpz_t variables.
    mpz_clears(a, b, i, NULL);
}

int main(void) {
    mpz_t x, n; mpz_inits(x, n, NULL);
    while (printf(">> ") && gmp_scanf("%Zu", n) != EOF) {
        fibonacci(x, n);
        gmp_printf("fibonacci(%Zu) = %Zu\n", n, x);
    }
    mpz_clears(x, n, NULL);
    return 0;
}
