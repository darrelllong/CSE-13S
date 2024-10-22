#include "code.h"
#include "defines.h"
#include <stdio.h>
#include <inttypes.h>

Code code_init(void) {
    Code c;
    c.top = 0;
    return c;
}

bool code_empty(Code *c) {
    return c->top == 0;
}

bool code_full(Code *c) {
    return c->top == ALPHABET;
}

uint32_t code_size(Code *c) {
    return c->top;
}

bool code_set_bit(Code *c, uint32_t i) {
    if (i < ALPHABET) {
        c->bits[i / 8] |= (1 << (i % 8));
        return true;
    }
    return false;
}

bool code_clr_bit(Code *c, uint32_t i) {
    if (i < ALPHABET) {
        c->bits[i / 8] &= ~(1 << (i % 8));
        return true;
    }
    return false;
}

bool code_get_bit(Code *c, uint32_t i) {
    if (i < ALPHABET) {
        return ((c->bits[i / 8] >> (i % 8)) & 1) == 1;
    }
    return false;
}

bool code_push_bit(Code *c, uint8_t x) {
    if (code_size(c) != ALPHABET) {
        if (x) {
            code_set_bit(c, c->top);
        } else {
            code_clr_bit(c, c->top);
        }
        c->top += 1;
        return true;
    }
    return false;
}

bool code_pop_bit(Code *c, uint8_t *x) {
    if (!code_empty(c)) {
        c->top -= 1;
        *x = code_get_bit(c, c->top);
        return true;
    }
    return false;
}

void code_print(Code *c) {
    printf("[");
    for (uint32_t i = 0; i < c->top; i += 1) {
        printf("%" PRIu8, code_get_bit(c, i));
    }
    printf("]\n");
}
