#include "twiddle.h"

nibble lo_nibble(byte b) { return b & 0x0f; }

nibble hi_nibble(byte b) { return (b & 0xf0) >> 4; }

byte octet(nibble hi, nibble lo) { return (hi << 4) | lo; }
