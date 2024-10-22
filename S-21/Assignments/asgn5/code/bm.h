#ifndef __BM_H__
#define __BM_H__

#include <stdint.h>

typedef struct BitMatrix BitMatrix;

BitMatrix *bm_create(uint32_t rows, uint32_t cols);

void bm_delete(BitMatrix **m);

uint32_t bm_rows(BitMatrix *m);

uint32_t bm_cols(BitMatrix *m);

void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c);

void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c);

uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c);

BitMatrix *bm_from_data(uint8_t byte, uint32_t length);

uint8_t bm_to_data(BitMatrix *m);

BitMatrix *bm_multiply(BitMatrix *A, BitMatrix *B);

void bm_print(BitMatrix *m);

#endif
