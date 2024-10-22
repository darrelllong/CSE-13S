#pragma one

#include <stdbool.h>
#include <stdint.h>

typedef struct BitMatrix BitMatrix;

BitMatrix *bm_create(uint32_t rows, uint32_t cols);

void bm_delete(BitMatrix *m);

uint32_t bm_rows(BitMatrix *m);

uint32_t bm_cols(BitMatrix *m);

void bm_set_bit(BitMatrix *m, uint32_t r, uint32_t c);

void bm_clr_bit(BitMatrix *m, uint32_t r, uint32_t c);

uint8_t bm_get_bit(BitMatrix *m, uint32_t r, uint32_t c);

void bm_print(BitMatrix *m);

bool bm_multiply(BitMatrix *, BitMatrix *, BitMatrix *);

bool bm_transpose(BitMatrix *, BitMatrix *);
