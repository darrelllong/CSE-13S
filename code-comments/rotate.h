#pragma once

// BSD License
#include "LICENSE.h"

#include <stdint.h>

static uint8_t rotL_8(uint8_t, int32_t);

static inline uint8_t rotR_8(uint8_t v, int32_t n) {
  if (n < 0) {
    return rotL_8(v, -n);
  } else {
    return (v >> (n & 0x07)) | (v << (8 - (n & 0x07)));
  }
}

static inline uint8_t rotL_8(uint8_t v, int32_t n) {
  if (n < 0) {
    return rotR_8(v, -n);
  } else {
    return (v << (n & 0x07)) | (v >> (8 - (n & 0x07)));
  }
}

static uint16_t rotL_16(uint16_t, int32_t);

static inline uint16_t rotR_16(uint16_t v, int32_t n) {
  if (n < 0) {
    return rotL_16(v, -n);
  } else {
    return (v >> (n & 0x0f)) | (v << (16 - (n & 0x0f)));
  }
}

static inline uint16_t rotL_16(uint16_t v, int32_t n) {
  if (n < 0) {
    return rotR_16(v, -n);
  } else {
    return (v << (n & 0x0f)) | (v >> (16 - (n & 0x0f)));
  }
}

static uint32_t rotL_32(uint32_t, int32_t);

static inline uint32_t rotR_32(uint32_t v, int32_t n) {
  if (n < 0) {
    return rotL_32(v, -n);
  } else {
    return (v >> (n & 0x1f)) | (v << (32 - (n & 0x1f)));
  }
}

static inline uint32_t rotL_32(uint32_t v, int32_t n) {
  if (n < 0) {
    return rotR_32(v, -n);
  } else {
    return (v << (n & 0x1f)) | (v >> (32 - (n & 0x1f)));
  }
}

static uint64_t rotL_64(uint64_t, int32_t);

static inline uint64_t rotR_64(uint64_t v, int32_t n) {
  if (n < 0) {
    return rotL_64(v, -n);
  } else {
    return (v >> (n & 0x1f)) | (v << (64 - (n & 0x3f)));
  }
}

static inline uint64_t rotL_64(uint64_t v, int32_t n) {
  if (n < 0) {
    return rotR_64(v, -n);
  } else {
    return (v >> (n & 0x1f)) | (v << (n & 0x1f)) | (v >> (64 - (n & 0x3f)));
  }
}
