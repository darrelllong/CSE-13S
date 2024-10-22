#ifndef __WORD_H__
#define __WORD_H__

#include <inttypes.h>

//
// Struct definition for a Word.
// A Word in LZW is just a byte array to hold characters.
//
// word:    The word byte array.
// length:  The length of the word.
//
typedef struct Word {
  uint8_t *word;
  uint64_t length;
} Word;

//
// Struct definition for a WordTable.
//
// entries:   An array of Word pointers.
//
typedef struct WordTable {
  Word *entries[UINT16_MAX];
} WordTable;

//
// Constructor for a Word.
//
// word:    The word byte array.
// length:  The length of the word.
//
Word *word_create(uint8_t *word, uint64_t length);

//
// Destructor for a Word.
//
// w:  The Word.
//
void word_delete(Word *w);

//
// Creates and returns a Word table, or an array of Words.
// Initialized with all ASCII characters.
// Static size of UINT16_MAX since 16-bit codes are used.
//
WordTable *wt_create(void);

//
// Resets a WordTable back to just the ASCII characters.
//
// wt:  The WordTable.
//
void wt_reset(WordTable *wt);

//
// Frees allocated memory for a WordTable.
//
// wt:  The WordTable.
//
void wt_delete(WordTable *wt);

//
// Prints out a Word.
//
// w:  The Word to print.
//
void word_print(Word *w);

//
// Prints out a WordTable.
//
// wt:  The WordTable to print.
//
void wt_print(WordTable *wt);

#endif
