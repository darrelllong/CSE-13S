#ifndef __ENDIAN_H__
#define __ENDIAN_H__

#include <inttypes.h>
#include <stdbool.h>

//
// Checks if byte order is big endian.
//
// returns: True if byte order is big endian, false otherwise.
//
static inline bool big_endian(void) {
  uint16_t word = 0x0001;
  uint8_t *bytes = (uint8_t *) &word;
  return bytes[1];
}

//
// Checks if byte order is little endian.
//
// returns: True if byte order is little endian, false otherwise.
//
static inline bool little_endian(void) {
  return !big_endian();
}

//
// Swaps endianness of a unsigned 16-bit integer.
//
// x:       Integer to swap endianness of.
// returns: Copy of x but with byte order swapped.
//
static inline uint16_t swap16(uint16_t x) {
  uint16_t result = 0;
  result |= (x & 0x00FF) << 8;
  result |= (x & 0xFF00) >> 8;
  return result;
}

//
// Swaps endianness of a unsigned 32-bit integer.
//
// x:       Integer to swap endianness of.
// returns: Copy of x but with byte order swapped.
//
static inline uint32_t swap32(uint32_t x) {
  uint32_t result = 0;
  result |= (x & 0x000000FF) << 24;
  result |= (x & 0x0000FF00) << 8;
  result |= (x & 0x00FF0000) >> 8;
  result |= (x & 0xFF000000) >> 24;
  return result;
}

//
// Swaps endianness of a unsigned 64-bit integer.
//
// x:       Integer to swap endianness of.
// returns: Copy of x but with byte order swapped.
//
static inline uint64_t swap64(uint64_t x) {
  uint64_t result = 0;
  result |= (x & 0x00000000000000FF) << 56;
  result |= (x & 0x000000000000FF00) << 40;
  result |= (x & 0x0000000000FF0000) << 24;
  result |= (x & 0x00000000FF000000) << 8;
  result |= (x & 0x000000FF00000000) >> 8;
  result |= (x & 0x0000FF0000000000) >> 24;
  result |= (x & 0x00FF000000000000) >> 40;
  result |= (x & 0xFF00000000000000) >> 56;
  return result;
}

#endif
