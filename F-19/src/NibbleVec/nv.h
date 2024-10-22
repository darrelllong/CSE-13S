#ifndef __NV_H__
#define __NV_H__

#include <inttypes.h>

typedef struct NibbleVector {
  uint32_t length;
  uint8_t *v;
} NibbleVector;

NibbleVector *nv_create(uint32_t nibble_len);

void nv_delete(NibbleVector *n);

void nv_set_nibble(NibbleVector *n, uint32_t i, uint8_t v);

void nv_clr_nibble(NibbleVector *n, uint32_t i);

uint8_t nv_get_nibble(NibbleVector *n, uint32_t i);

void nv_print(NibbleVector *n);

#endif
