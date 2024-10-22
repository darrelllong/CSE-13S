#include "util.h"
#include "word.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Constructor for a Word.
//
// word:        The word byte array.
// length:    The length of the word.
//
Word *word_create(uint8_t *word, uint64_t length) {
  Word *w = (Word *)malloc(sizeof(Word));
  ASSERT(w, "%s\n", "Failed to malloc() word.");

  w->length = length;
  w->word = (uint8_t *)malloc(length * sizeof(uint8_t));
  ASSERT(w->word, "%s\n", "Failed to malloc() word byte array.");

  memcpy(w->word, word, length);
  return w;
}

//
// Destructor for a Word.
//
// w:  The Word.
//
void word_delete(Word *w) {
  free(w->word);
  w->word = NULL;
  free(w);
  return;
}

//
// Creates and returns a Word table, or an array of Words.
// Initialized with all ASCII characters.
// Static size of UINT16_MAX since 16-bit codes are used.
//
WordTable *wt_create(void) {
  WordTable *wt = (WordTable *)calloc(1, sizeof(WordTable));
  ASSERT(wt, "%s\n", "Failed to calloc() WordTable.");

  for (uint16_t i = 0; i < ALPHABET; i += 1) {
    wt->entries[i] = word_create((uint8_t *)&i, 1);
  }

  return wt;
}

//
// Resets a WordTable back to just the ASCII characters.
//
// wt:  The WordTable.
//
void wt_reset(WordTable *wt) {
  for (uint32_t i = ALPHABET; i < UINT16_MAX; i += 1) {
    if (wt->entries[i]) {
      word_delete(wt->entries[i]);
      wt->entries[i] = NULL;
    }
  }

  return;
}

//
// Frees allocated memory for a WordTable.
//
// wt:  The WordTable.
//
void wt_delete(WordTable *wt) {
  for (uint32_t i = 0; i < UINT16_MAX; i += 1) {
    if (wt->entries[i]) {
      word_delete(wt->entries[i]);
      wt->entries[i] = NULL;
    }
  }

  free(wt);
  return;
}

//
// Prints out a Word.
//
// w:  The Word to print.
//
void word_print(Word *w) {
  printf("\"");

  for (uint64_t i = 0; i < w->length; i += 1) {
    printf("%c", (char)w->word[i]);
  }

  printf("\"\n");
  return;
}

//
// Prints out a WordTable.
//
// wt:  The WordTable to print.
//
void wt_print(WordTable *wt) {
  for (uint32_t i = 0; i < UINT16_MAX; i += 1) {
    if (wt->entries[i]) {
      printf("Code: %" PRIu32 "\tWord: ", i);
      word_print(wt->entries[i]);
    }
  }

  return;
}
