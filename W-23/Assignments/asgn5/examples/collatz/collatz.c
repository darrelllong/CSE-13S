#include <stdio.h>
#include <gmp.h>

// Computes number of steps in Collatz sequence starting from n.
// Computed steps stored in x.
void collatz(mpz_t x, mpz_t n) {
    mpz_t m; mpz_init_set(m, n); // m = n

    // for (x = 0; m > 1; x += 1) { ... }
    for (mpz_set_ui(x, 0); mpz_cmp_ui(m, 1) > 0; mpz_add_ui(x, x, 1)) {
        if (mpz_even_p(m)) {
            mpz_fdiv_q_ui(m, m, 2);     // m /= 2
        } else {
            mpz_mul_ui(m, m, 3);        // m *= 3
            mpz_add_ui(m, m, 1);        // m += 1
        }
    }

    mpz_clear(m);
}

int main(void) {
    mpz_t x, n; mpz_inits(x, n, NULL);
    while (printf(">> ") && gmp_scanf("%Zu", n) != EOF) {
        collatz(x, n);
        gmp_printf("collatz(%Zu) = %Zu\n", n, x);
    }
    mpz_clears(x, n, NULL);
    return 0;
}
