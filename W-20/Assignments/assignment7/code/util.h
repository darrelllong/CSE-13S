#ifndef __UTIL_H__
#define __UTIL_H__

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>

#define ALPHABET  256         // The alphabet is the ASCII table.
#define BYTE      8           // Byte is 8 bits.
#define SHORT     16          // Short is 16 bits.
#define BLOCK     4096        // Block is 4KB.
#define MAGIC     0x8badbeef  // Unique encoder/decoder magic number.

//
// Calculates number of bytes needed to represent a certain number of bits.
//
// bits:      Number of bits.
// returns:   Number of bytes needed.
//
uint64_t bytes(uint64_t bits);

//
// Calculates the least number of bits needed to represent a uint16_t.
//
// n:       Number to calculate bitwidth of.
// returns: The calculated bitwidth.
//
uint8_t bitwidth(uint16_t n);

//
// Checks if a certain condition is satisfied.
// If not satisfied, the supplied format string is printed to stderr.
//
// cond:    Condition that should be true.
// fmt:     Format string to print if condition isn't met.
// ...:     Variable arguments for format string.
// returns: Void.
//
void check(bool cond, char *fmt, ...);

#endif
