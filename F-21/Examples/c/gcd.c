#include <stdio.h>
#include <inttypes.h>

uint64_t steps = 0;

uint64_t gcd_naive(uint64_t a, uint64_t b) {
    steps = 0;
    uint64_t min = a < b ? a : b;
    for (uint64_t i = min; i > 1; i -= 1) {
        steps += 1;
        if (a % i == 0 && b % i == 0) {
            return i;
        }
    }
    return 1;
}

uint64_t gcd_mod(uint64_t a, uint64_t b) {
    steps = 0;
    while (b != 0) {
        steps += 1;
        uint64_t t = a; a = b;
        b = t % b;
    }
    return a;
}

int main(void) {
    uint64_t a, b;
    printf("a b ?? ");
    scanf("%" SCNu64 "%" SCNu64, &a, &b);
    printf("gcd(%" PRIu64 ", %" PRIu64 ") = %" PRIu64, a, b, gcd_naive(a, b));
    printf(" (%" PRIu64 " steps)\n", steps);
    printf("gcd(%" PRIu64 ", %" PRIu64 ") = %" PRIu64, a, b, gcd_mod(a, b));
    printf(" (%" PRIu64 " steps)\n", steps);
    return 0;
}
