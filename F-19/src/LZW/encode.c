#include "io.h"
#include "trie.h"
#include "util.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

extern uint64_t max_word_len;
extern uint8_t bitwidth;

//
// Handles LZW compression algorithm.
// Compresses contents of the input file to the output file.
//
// infile:     Input file to compress.
// outfile:    Output file where compressed data goes.
//
void encode(int infile, int outfile) {
  FileHeader header = { 0 };

  // Get file size and protection bits.
  struct stat statbuf;
  fstat(infile, &statbuf);
  header.file_size = statbuf.st_size;
  header.protection = statbuf.st_mode;
  header.bitwidth = bitwidth;

  // Change permissions of output file based on protection bits.
  if (outfile != STDOUT_FILENO) {
    int status = fchmod(outfile, header.protection);
    ASSERT(status != -1, "%s\n", "Failed to change mode of outfile.");
  }

  // Get file size and write header to outfile.
  write_header(outfile, &header);

  // Initialize Trie and make copy of its root.
  TrieNode *root = trie_create();
  TrieNode *curr_node = root;

  // Maximum code value is max number representable by bitwidth bits.
  // Placed in uint32_t to accomodate max bitwidth of 16 bits.
  uint32_t max_code_val = (1 << bitwidth) - 1;

  // Things to keep track of:
  //  - next available code (starts at 256 due to the ASCIIs)
  //  - number of encoded characters
  //  - current word length
  uint16_t code = 256;
  uint64_t encoded = 0;
  uint64_t curr_word_len = 0;

  while (encoded != header.file_size) {
    uint8_t curr_char = next_char(infile);
    curr_word_len += 1;

    // Is the next character there? (Can we step down?)
    TrieNode *next_node = trie_step(curr_node, curr_char);

    if (next_node) {
      // Move onto next character.
      curr_node = next_node;
    } else {
      // Buffer code for writing.
      buffer_code(outfile, curr_node->code, log2_64(code) + 1);

      // Add character for unseen prefix to Trie and update current prefix.
      curr_node->children[curr_char] = trie_node_create(code++);
      curr_node = root->children[curr_char];

      // Update max word length, reset current word length.
      max_word_len = max(max_word_len, curr_word_len);
      curr_word_len = 1;
    }

    encoded += 1;

    // Have we exhausted all codes?
    if (code == max_code_val) {
      trie_reset(root);
      curr_node = root->children[curr_char];
      code = 256;
    }
  }

  // Buffer code if we are still matching a prefix.
  if (curr_word_len > 0) {
    buffer_code(outfile, curr_node->code, log2_64(code) + 1);
  }

  // Flush any codes in our buffer.
  flush_code(outfile);

  // Clean up and leave.
  trie_delete(root);
  return;
}

