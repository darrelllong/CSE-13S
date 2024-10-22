#ifndef __WORD_H__
#define __WORD_H__

#include <inttypes.h>

void words_init(void);

void words_add(uint16_t code, uint8_t sym, uint16_t next);

uint32_t chain_codes(uint16_t code);

uint8_t get_sym(void);

#endif

