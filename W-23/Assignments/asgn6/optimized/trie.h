#ifndef __TRIE_H__
#define __TRIE_H__

#include <inttypes.h>

void trie_init(void);

void trie_add(uint16_t prev_code, uint8_t sym, uint16_t code);

uint16_t trie_step(uint16_t prev_code, uint8_t sym);

#endif
