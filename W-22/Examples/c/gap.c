#include <math.h>
#include <stdint.h>
#include <stdio.h>

static inline uint64_t max_k(uint64_t n) { return (uint64_t) log(2 * n + 3) / log(3); }

static inline uint64_t power(uint64_t a, uint64_t d) {
    uint64_t v = 1;
    for (uint64_t p = a; d > 0; d >>= 1) {
        if (d & 0x1) {
            v *= p;
        }
        p *= p;
    }
    return v;
}

static inline uint64_t gap(uint64_t k) { return (power(3, k) - 1) / 2; }

int main(void) {
    int n;
    printf("?? ");
    scanf("%d", &n);
    int max = max_k(n);
        printf("gap (%d) =", max);
    for (int i = max; i > 0; i -= 1) {
        printf(" %llu", gap(i));
    }
    printf("\n");
}
