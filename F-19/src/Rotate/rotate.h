#ifndef _ROT_DL
#define _ROT_DL

#include <stdint.h>

static inline uint8_t rotR_8(uint8_t v, uint32_t n) {
    return (v >> (n & 0x07)) | (v << (8 - (n & 0x07)));
}

static inline uint8_t rotL_8(uint8_t v, uint32_t n) {
    return (v << (n & 0x07)) | (v >> (8 - (n & 0x07)));
}

static inline uint16_t rotR_16(uint16_t v, uint32_t n) {
    return (v >> (n & 0x0f)) | (v << (16 - (n & 0x0f)));
}

static inline uint16_t rotL_16(uint16_t v, uint32_t n) {
    return (v << (n & 0x0f)) | (v >> (16 - (n & 0x0f)));
}

static inline uint32_t rotR_32(uint32_t v, uint32_t n) {
    return (v >> (n & 0x1f)) | (v << (32 - (n & 0x1f)));
}

static inline uint32_t rotL_32(uint32_t v, uint32_t n) {
    return (v << (n & 0x1f)) | (v >> (32 - (n & 0x1f)));
}

static inline uint64_t rotR_64(uint64_t v, uint32_t n) {
    return (v >> (n & 0x1f)) | (v << (64 - (n & 0x3f)));
}

static inline uint64_t rotL_64(uint64_t v, uint32_t n) {
    return (v << (n & 0x1f)) | (v >> (64 - (n & 0x3f)));
}

#endif
