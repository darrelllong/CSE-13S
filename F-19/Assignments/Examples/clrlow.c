#include <inttypes.h>
#include <stdio.h>

uint64_t clear_low(uint64_t value, uint64_t k) {
    const uint64_t max_bits = 64;
    return value & ~(((uint64_t)1 << (k % max_bits)) - 1);
}

uint64_t leave_low(uint64_t value, uint64_t k) {
    const uint64_t max_bits = 64;
    return value & (((uint64_t)1 << (k % max_bits)) - 1);
}

int main(void) {
    const uint64_t val = 0x1234567890abcdef;
    uint64_t clr;
    printf("How many? ");
    scanf("%" PRIu64, &clr);
    printf("clear_low(%" PRIx64 ", %" PRIu64 ") = %" PRIx64 "\n", val, clr,
           clear_low(val, clr));
    printf("leave_low(%" PRIx64 ", %" PRIu64 ") = %" PRIx64 "\n", val, clr,
           leave_low(val, clr));
    return 0;
}
