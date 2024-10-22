#pragma once

#include <stdint.h>
#include <sys/types.h>

#define SHA3_DIGEST_SIZE 32 // SHA3 256-bit digest in bytes.

void sha3_digest(uint8_t digest[], uint8_t data[], size_t n);
