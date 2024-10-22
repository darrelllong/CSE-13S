#include "io.h"
#include "util.h"
#include "word.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

extern uint64_t max_word_len;

//
// Handles LZW decompression algorithm.
// Decompresses contents of the input file to the output file.
//
// infile:     Input file to decompress.
// outfile:    Output file where decompressed data goes.
//
void decode(int infile, int outfile) {
  FileHeader header = { 0, 0, 0, 0 };

  // Read in header which verifies magic number.
  read_header(infile, &header);

  // Change file permissions based on protection bits.
  if (outfile != STDOUT_FILENO) {
    int status = fchmod(outfile, header.protection);
    ASSERT(status != -1, "%s\n", "Failed to change outfile permission bits.");
  }

  // Create WordTable.
  WordTable *wt = wt_create();

  // Keep track of previous and current code number.
  uint16_t curr_code = 0;
  uint16_t prev_code = 0;

  // Byte arrays to keep track of previous and current word.
  uint64_t word_size = BLOCK;
  uint8_t *prev_word = (uint8_t *)calloc(word_size, sizeof(uint8_t));
  uint8_t *curr_word = (uint8_t *)calloc(word_size, sizeof(uint8_t));
  ASSERT(prev_word && curr_word, "%s\n", "Failed to calloc() buffers.");

  // Things to keep track of:
  //  - next available code (starts at 256 due to ASCIIs)
  //  - number of decoded characters
  //  - whether or not the Trie is in its reset state
  uint16_t code = 256;
  uint64_t decoded = 0;
  bool reset = true;

  while (decoded != header.file_size) {
    // Get next code (bitwidth determined by code after the next available).
    curr_code = next_code(infile, log2_64(code + 1) + 1);

    // Current entry is NULL if we haven't seen the current code yet.
    Word *curr_entry = wt->entries[curr_code];

    if (reset) {
      // First code decodes to one character, so just add it to output.
      buffer_word(outfile, curr_entry);

      // Set previous word to the first decoded character.
      prev_word[0] = curr_entry->word[0];

      // Increment decoded count and turn off reset flag.
      decoded += 1;
      reset = false;
    } else if (curr_entry) {
      // Update current word with memcpy() (apparently it has optimizations).
      memcpy(curr_word, curr_entry->word, curr_entry->length);

      // Get previous word entry and copy into previous word.
      Word *prev_entry = wt->entries[prev_code];
      ASSERT(prev_entry, "%s\n", "Failed to find previous entry.");
      memcpy(prev_word, prev_entry->word, prev_entry->length);

      // New word is previous word appended with first of current word.
      uint8_t new_word[prev_entry->length + 1];
      memcpy(new_word, prev_entry->word, prev_entry->length);
      new_word[prev_entry->length] = curr_word[0];

      // Reallocate space for previous and current word if necessary.
      if (prev_entry->length + 1 == word_size) {
        word_size *= 2;
        prev_word = realloc(prev_word, word_size * sizeof(uint8_t));
        curr_word = realloc(curr_word, word_size * sizeof(uint8_t));
        ASSERT(prev_word && curr_word, "%s\n", "Failed to realloc() buffers.");
      }

      // Add new code number and its word into hash table.
      wt->entries[code++] = word_create(new_word, prev_entry->length + 1);

      // Buffer current word for writing.
      buffer_word(outfile, curr_entry);

      // Update longest word length and increment decoded count.
      max_word_len = max(max_word_len, prev_entry->length + 1);
      decoded += curr_entry->length;
    } else {
      // Get previous word entry and copy into previous word.
      Word *prev_entry = wt->entries[prev_code];
      ASSERT(prev_entry, "%s\n", "Failed to find previous entry.");
      memcpy(prev_word, prev_entry->word, prev_entry->length);

      // Current word is previous word + first of previous word.
      memcpy(curr_word, prev_word, prev_entry->length);
      curr_word[prev_entry->length] = prev_word[0];

      // Reallocate space for previous and current word if necessary.
      if (prev_entry->length + 1 == word_size) {
        word_size *= 2;
        prev_word = realloc(prev_word, word_size * sizeof(uint8_t));
        curr_word = realloc(curr_word, word_size * sizeof(uint8_t));
        ASSERT(prev_word && curr_word, "%s\n", "Failed to realloc() buffers");
      }

      // Create missing entry.
      Word *temp_entry = word_create(curr_word, prev_entry->length + 1);

      // Add missing entry to table.
      wt->entries[code++] = temp_entry;

      // Buffer word for writing.
      buffer_word(outfile, temp_entry);

      decoded += temp_entry->length;
    }

    prev_code = curr_code;

    // Has the compressor run out of codes? (We predict one code earlier)
    if (code == UINT16_MAX - 1) {
      wt_reset(wt);
      code = 256;
      reset = true;
    }
  }

  // Flush any remaining characters.
  flush_words(outfile);

  // Clean up and leave.
  wt_delete(wt);
  wt = NULL;
  free(prev_word);
  prev_word = NULL;
  free(curr_word);
  curr_word = NULL;
  return;
}
