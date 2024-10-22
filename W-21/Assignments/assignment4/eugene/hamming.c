#include "hamming.h"
#include "bitmatrix.h"

static BitMatrix *generator(void) {
    BitMatrix *G = bm_create(4, 8);
    for (uint32_t i = 0; i < bm_rows(G); i += 1) {
        bm_set_bit(G, i, i);
    }
    for (uint32_t r = 0; r < bm_rows(G); r += 1) {
        for (uint32_t c = 4; c < bm_cols(G); c += 1) {
            r + 4 == c ? bm_clr_bit(G, r, c) : bm_set_bit(G, r, c);
        }
    }
    return G;
}

static BitMatrix *parity(void) {
    BitMatrix *Ht = bm_create(8, 4);
    for (uint32_t r = 0; r < bm_cols(Ht); r += 1) {
        for (uint32_t c = 0; c < bm_cols(Ht); c += 1) {
            r == c ? bm_clr_bit(Ht, r, c) : bm_set_bit(Ht, r, c);
        }
    }
    for (uint32_t i = 4; i < bm_rows(Ht); i += 1) {
        bm_set_bit(Ht, i, i - 4);
    }
    return Ht;
}

uint8_t encode(uint8_t msg) {
    BitMatrix *G = generator();
    BitMatrix *m = bm_from_data(msg, 4);
    BitMatrix *c = bm_multiply(m, G);
    uint8_t code = bm_to_data(c);
    bm_delete(&G);
    bm_delete(&m);
    bm_delete(&c);
    return code;
}

int decode(uint8_t code, uint8_t *msg) {
    int status = 0; // -2 is fine, -1 is error, >= 0 is corrected.

    static const int syndrome[16] = {
        -2, // 0b0000 -> happy
        4,  // 0b0001 -> row 4
        5,  // 0b0010 -> row 5
        -1, // 0b0011 -> error
        6,  // 0b0100 -> row 6
        -1, // 0b0101 -> error
        -1, // 0b0110 -> error
        3,  // 0b0111 -> row 3
        7,  // 0b1000 -> row 7
        -1, // 0b1001 -> error
        -1, // 0b1010 -> error
        2,  // 0b1011 -> row 2
        -1, // 0b1100 -> error
        1,  // 0b1101 -> row 1
        0,  // 0b1110 -> row 0
        -1  // 0b1111 -> error
    };

    *msg = code & 0x0F;

    BitMatrix *Ht = parity();
    BitMatrix *c = bm_from_data(code, 8);
    BitMatrix *m = bm_multiply(c, Ht);

    if ((status = syndrome[bm_to_data(m)]) >= 0) {
        *msg = (code ^= (1 << syndrome[bm_to_data(m)])) & 0x0F;
    }

    bm_delete(&Ht);
    bm_delete(&c);
    bm_delete(&m);
    return status;
}
