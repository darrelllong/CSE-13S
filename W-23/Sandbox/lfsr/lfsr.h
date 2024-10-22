#pragma once

#include <inttypes.h>

#define FEEDBACK 0xc000000000000001 // Primitive polynomial x^63 + x^62 + x^0

uint64_t lfsr(uint64_t *);

uint8_t randomByte(uint64_t *);

void warmLFSR(uint64_t *);
