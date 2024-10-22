#include "trie.h"
#include "util.h"
#include <inttypes.h>
#include <string.h>

// Each index stores a non-zero code.
// A symbol doesn't exist in the trie if its code is zero.
static uint16_t table[MAX_CODE][ALPHABET];

void trie_init(void) {
  memset(table, 0, sizeof(table));
  return;
}

void trie_add(uint16_t prev_code, uint8_t sym, uint16_t code) {
  table[prev_code][sym] = code;
  return;
}

uint16_t trie_step(uint16_t prev_code, uint8_t sym) {
  return table[prev_code][sym];
}
