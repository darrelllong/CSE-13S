#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "isprime.h"

int main(void) {
    primeInitialize();
    for (uint64_t i = 0; i < (uint64_t) 1 << 32; i += 1) {
        if (isPrime(i, 1)) {
            printf("%" PRIu64 ": P", i);
            bool prime = true;
            for (uint64_t k = 2; prime && k <= 10; k += 1) {
                putchar(isPrime(i, k) ? 'P' : (prime = false, 'C'));
            }
            putchar('\n');
        }
    }
    primeFinalize();
    return 0;
}
