#include "hamming.h"

uint8_t encode(BitMatrix *G, uint8_t msg) {
    BitMatrix *m = bm_from_data(msg, 4);
    BitMatrix *c = bm_multiply(m, G);
    uint8_t code = bm_to_data(c);
    bm_delete(&m);
    bm_delete(&c);
    return code;
}

HAM_STATUS decode(BitMatrix *Ht, uint8_t code, uint8_t *msg) {
    HAM_STATUS status = HAM_OK;

    static const int syndrome[16] = {
        HAM_OK,  // 0b0000 -> happy
        4,       // 0b0001 -> row 4
        5,       // 0b0010 -> row 5
        HAM_ERR, // 0b0011 -> error
        6,       // 0b0100 -> row 6
        HAM_ERR, // 0b0101 -> error
        HAM_ERR, // 0b0110 -> error
        3,       // 0b0111 -> row 3
        7,       // 0b1000 -> row 7
        HAM_ERR, // 0b1001 -> error
        HAM_ERR, // 0b1010 -> error
        2,       // 0b1011 -> row 2
        HAM_ERR, // 0b1100 -> error
        1,       // 0b1101 -> row 1
        0,       // 0b1110 -> row 0
        HAM_ERR  // 0b1111 -> error
    };

    BitMatrix *c = bm_from_data(code, 8);
    BitMatrix *m = bm_multiply(c, Ht);

    switch (syndrome[bm_to_data(m)]) {
    case HAM_ERR:
        status = HAM_ERR;
        break;
    case HAM_OK:
        status = HAM_OK;
        *msg   = code & 0x0F;
        break;
    default:
        status = HAM_CORRECT;
        *msg   = (code ^= (1 << syndrome[bm_to_data(m)])) & 0x0F;
        break;
    }

    bm_delete(&c);
    bm_delete(&m);
    return status;
}
