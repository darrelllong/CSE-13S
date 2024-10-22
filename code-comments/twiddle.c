// BSD License
#include "LICENSE.h"

#include <inttypes.h>
#include <stdio.h>

uint64_t set_bit(uint64_t w, uint8_t v) {
    return w | ((uint64_t) 0x1 << (v % 64));
}

uint64_t clr_bit(uint64_t w, uint8_t v) {
    return w & ~((uint64_t) 0x1 << (v % 64));
}

uint64_t get_bit(uint64_t w, uint8_t v) {
    return (w & ((uint64_t) 0x1 << (v % 64))) >> (v % 64);
}

void prn_bit(uint64_t w) {
    for (int64_t i = 63; i >= 0; i -= 1) {
        printf("%c", '0' + (unsigned char) get_bit(w, i));
    }
    printf("\n");
}

int main(void) {
    uint64_t w = 0;
    int b;

    while (scanf("%d", &b) != EOF && b >= 0) {
        w = set_bit(w, b);
        prn_bit(w);
    }

    while (scanf("%d", &b) != EOF) {
        w = clr_bit(w, b);
        prn_bit(w);
    }
    return 0;
}
