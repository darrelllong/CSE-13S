#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <inttypes.h>

uint64_t random_bits(uint64_t b) {
    uint64_t r;
    do {
        puts("bonk");
        r = random() & (1 << b) - 1;
    } while (r < (1 << (b - 1)));
    return r;
}

int main(void) {
    srandom(time(NULL));
    uint64_t b;
    printf("Bits: ");
    scanf("%" SCNu64, &b);
    uint64_t r = random_bits(b);
    printf("%" PRIu64 " (0x%" PRIX64 ")\n", r, r);
    return 0;
}
