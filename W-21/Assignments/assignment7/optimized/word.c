#include "util.h"
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>

// Each code (index) in the array refers to a symbol it was appended with.
static uint8_t syms[MAX_CODE];

// Each code (index) in the array refers to a previous code in the array.
static uint16_t codes[MAX_CODE];

// Quick macro-defined stack of codes to hold the chain of codes.
static uint16_t stack[MAX_CODE];
static uint32_t ptr = 0;

#define stack_push(x) (stack[ptr++] = x)
#define stack_pop()   (stack[--ptr])

void words_init(void) {
  memset(syms, 0, MAX_CODE);
  memset(codes, 0, MAX_CODE);
  return;
}

void words_add(uint16_t code, uint8_t sym, uint16_t next) {
  // The symbol used by the next code is sym.
  // The code referred to by the next code is code.
  syms[next] = sym;
  codes[next] = code;
  return;
}

uint32_t chain_codes(uint16_t code) {
  // Start the chain off with the current code.
  // Keep a count of the number of chained codes, and thus, symbols.
  uint16_t chain = code;
  uint32_t count = 0;

  // Each code in a pair refers to a previous code.
  // Chained codes are pushed onto the stack.
  // The empty code is the exception; we don't care about its symbol.
  while (chain != EMPTY_CODE) {
    stack_push(chain);
    chain = codes[chain];
    count += 1;
  }

  return count;
}

uint8_t get_sym(void) {
  return syms[stack_pop()];
}

