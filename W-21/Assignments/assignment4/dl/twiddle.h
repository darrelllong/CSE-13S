#ifndef __TWIDDLE_H
#define __TWIDDLE_H
#include <stdint.h>

typedef uint8_t byte;
typedef uint8_t nibble;

nibble low_nibble(byte);

nibble high_nibble(byte);

byte octet(nibble, nibble);

#endif
