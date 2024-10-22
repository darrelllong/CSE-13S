#include "util.h"
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t bytes(uint64_t bits) {
    return ((bits % 8) || (!bits)) ? bits / 8 + 1 : bits / 8;
}

uint8_t bitwidth(uint16_t n) {
    uint8_t msb_ind = 0;

    while (n > 1) {
        msb_ind += 1;
        n >>= 1;
    }

    return msb_ind + 1;
}

void check(bool cond, char *fmt, ...) {
    if (!cond) {
        va_list arg;
        va_start(arg, fmt);
        vfprintf(stderr, fmt, arg);
        va_end(arg);
        exit(EXIT_FAILURE);
    }
}
