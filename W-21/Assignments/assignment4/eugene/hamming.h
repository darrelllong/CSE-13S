#ifndef __HAMMING_H__
#define __HAMMING_H__

#include <stdbool.h>
#include <stdint.h>

uint8_t encode(uint8_t msg);

int decode(uint8_t code, uint8_t *msg);

#endif
