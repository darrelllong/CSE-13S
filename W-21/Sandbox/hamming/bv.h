#ifndef _BVector
#define _BVector

#include <stdint.h>

typedef struct bitV bitV;

bitV *bv_new(uint32_t length);

void bv_del(bitV **bv);

uint8_t bv_set_all(bitV *bv);

uint8_t bv_set_bit(bitV *bv, uint32_t index);

uint8_t bv_clr_bit(bitV *bv, uint32_t index);

uint8_t bv_get_val(bitV *bv, uint32_t index);

uint32_t bv_get_len(bitV *bv);

#endif
