#ifndef _ROT_DL
#define _ROT_DL
#include <stdint.h>

#define rolr(v,k) (typeof (v)) ({ \
const uint8_t bits = sizeof(v) * 8; \
const uint8_t mask = bits - 1; \
(v >> (k & mask)) | (v << (bits - (k & mask))); \
}) 

#define roll(v,k) (typeof (v)) ({ \
const uint8_t bits = sizeof(v) * 8; \
const uint8_t mask = bits - 1; \
(v << (k & mask)) | (v >> (bits - (k & mask))); \
}) 
#endif
