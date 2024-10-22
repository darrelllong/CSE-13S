#include "code.h"
#include "defines.h"
#include <stdio.h>

Code code_init(void) {
    Code c;
    c.top = 0;
    return c;
}

uint32_t code_size(Code *c) {
    return c->top;
}

bool code_push_bit(Code *c, uint8_t x) {
    if (code_size(c) != ALPHABET) {
        if (x) {
            SETBIT(c->bits, c->top);
        } else {
            CLRBIT(c->bits, c->top);
        }
        c->top += 1;
        return true;
    }
    return false;
}

bool code_pop_bit(Code *c, uint8_t *x) {
    if (code_size(c) > 0) {
        c->top -= 1;
        *x = GETBIT(c->bits, c->top);
        return true;
    }
    return false;
}

void code_print(Code *c) {
    printf("[");
    for (uint32_t i = 0; i < c->top; i += 1) {
        printf(GETBIT(c->bits, i) ? "1" : "0");
    }
    printf("]\n");
}
