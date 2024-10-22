#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "isprime.h"

int main(void) {
    primeInitialize();
    for (uint64_t i = 0; i < 100; i += 1) {
        printf("%" PRIu64 " is %s\n", i, isPrime(i, 10) ? "prime" : "composite");
    }
    uint64_t n = 2;
    printf("Candidate: ");
    scanf("%" SCNi64, &n);
    for (uint64_t k = 1; k < 50; k += 1) {
        printf("%" PRIu64 " %s\n", k, isPrime(n, k) ? "prime" : "composite");
    }
    primeFinalize();
    return 0;
}
