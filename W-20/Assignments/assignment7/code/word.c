#include "code.h"
#include "util.h"
#include "word.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Constructor for a word.
//
// syms:    Array of symbols a Word represents.
// len:     Length of the array of symbols.
// returns: Pointer to a Word that has been allocated memory.
//
Word *word_create(uint8_t *syms, uint64_t len) {
  Word *w = (Word *) malloc(sizeof(Word));
  check(w, "Failed to malloc() word.\n");

  w->len = len;
  w->syms = (uint8_t *) malloc(len * sizeof(uint8_t));
  check(w->syms, "Failed to malloc() word byte array.\n");

  memcpy(w->syms, syms, len);
  return w;
}

//
// Constructs a new Word from the specified Word appended with a symbol.
// The Word specified to append to may be empty.
// If the above is the case, the new Word should contain only the symbol.
//
// w:       Word to append to.
// sym:     Symbol to append.
// returns: New Word which represents the result of appending.
//
Word *word_append_sym(Word *w, uint8_t sym) {
  check(w, "what\n");

  Word *appended = (Word *) malloc(sizeof(Word));
  check(appended, "Failed to malloc() word.\n");

  appended->len = w->len + 1;
  appended->syms = (uint8_t *) malloc(appended->len * sizeof(uint8_t));
  check(appended->syms, "Failed to malloc() word byte array.\n");

  if (w->len) {
    memcpy(appended->syms, w->syms, w->len);
  }

  appended->syms[w->len] = sym;
  return appended;
}

//
// Destructor for a Word.
//
// w:       Word to free memory for.
// returns: Void.
//
void word_delete(Word *w) {
  if (w->syms) {
    free(w->syms);
    w->syms = NULL;
  }

  free(w);
  return;
}

//
// Creates a new WordTable, which is an array of Words.
// A WordTable has a pre-defined size of MAX_CODE (UINT16_MAX).
// This is because indices are 16-bit integers.
// A WordTable is initialized with a single Word at index EMPTY_CODE.
// This Word represents the empty string, thus meaning a length of zero.
//
// returns: Initialized WordTable.
//
WordTable *wt_create(void) {
  WordTable *wt = (WordTable *) calloc(MAX_CODE, sizeof(Word *));
  check(wt, "Failed to calloc() word table.\n");

  wt[EMPTY_CODE] = (Word *) calloc(1, sizeof(Word));
  check(wt[EMPTY_CODE], "Failed to calloc() empty word.\n");

  return wt;
}

//
// Resets a WordTable to just having the initial empty Word at EMPTY_CODE.
//
// wt:      WordTable to reset.
// returns: Void.
//
void wt_reset(WordTable *wt) {
  for (uint32_t i = START_CODE; i < MAX_CODE; i += 1) {
    if (wt[i]) {
      word_delete(wt[i]);
      wt[i] = NULL;
    }
  }

  return;
}

//
// Deletes an entire WordTable.
// All Words in the WordTable must be deleted as well.
//
// wt:      WordTable to free memory for.
// returns: Void.
//
void wt_delete(WordTable *wt) {
  for (uint32_t i = EMPTY_CODE; i < MAX_CODE; i += 1) {
    if (wt[i]) {
      word_delete(wt[i]);
      wt[i] = NULL;
    }
  }

  free(wt);
  return;
}
