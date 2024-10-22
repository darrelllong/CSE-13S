#include "endian.h"
#include "io.h"
#include "util.h"
#include "word.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern int exit_status;

//
// Reads a file header from the input file.
//
// infile:     Input file descriptor.
// header:     The file header struct to read into.
//
void read_header(int infile, FileHeader *header) {
  read_buffer(infile, (uint8_t *)header, sizeof(FileHeader));

  // Verify magic number.
  if (header->magic != MAGIC && swap32(header->magic) != MAGIC) {
    errprint("File not compressed by this program.\n");
    exit(exit_status);
  }

  // Fix endianness if needed.
  if (is_big()) {
    header->magic = swap32(header->magic);
    header->file_size = swap64(header->file_size);
  }

  return;
}

//
// Writes a file header to the output file and returns original file size.
//
// outfile:    Output file descriptor.
// header:     FileHeader to set values in and write.
//
void write_header(int outfile, FileHeader *header) {
  // Add the magic number.
  header->magic = MAGIC;

  // Fix endianness for header fields if needed.
  if (is_big()) {
    header->magic = swap32(header->magic);
    header->file_size = swap64(header->file_size);
  }

  write_buffer(outfile, (uint8_t *)header, sizeof(FileHeader));
  return;
}

// Buffer for storing code bits in.
static uint8_t bit_buf[BLOCK] = { 0 };
static uint64_t bit_cnt = 0;

// Buffer for storing characters in.
static uint8_t char_buf[BLOCK] = { 0 };
static uint64_t char_cnt = 0;

// Variables to keep track of statistics for verbose flag.
uint64_t bit_total = 0;
uint64_t char_total = 0;

//
// Returns next character, or byte, from the input file.
// 4KB of characters are read when needed into a character buffer.
// This is only called once for each byte in the input file.
//
// infile:     Input file descriptor
//
uint8_t next_char(int infile) {
  char_total += 1;

  if (!char_cnt) {
    read_buffer(infile, char_buf, BLOCK);
  }

  uint8_t c = char_buf[char_cnt];
  char_cnt = (char_cnt + 1) % BLOCK;

  return c;
}

//
// Buffers a code into the code buffer for writing.
// The binary of the code is placed into the buffer in reverse.
//
// outfile:   Output file descriptor.
// code:      The code to buffer.
// bit_len:   The length in bits of the code.
//
void buffer_code(int outfile, uint16_t code, uint8_t bit_len) {
  bit_total += bit_len;

  for (uint8_t i = 0; i < bit_len; i += 1) {
    if (code & (1 << (i % HALFWORD))) {
      bit_buf[bit_cnt / BYTE] |= (1 << (bit_cnt % BYTE));
    } else {
      bit_buf[bit_cnt / BYTE] &= ~(1 << (bit_cnt % BYTE));
    }

    bit_cnt += 1;

    if (bit_cnt / BYTE == BLOCK) {
      write_buffer(outfile, bit_buf, BLOCK);
      bit_cnt = 0;
    }
  }

  return;
}

//
// Flushes any remaining codes in the buffer to the output file.
//
// outfile:    Output file descriptor.
//
void flush_codes(int outfile) {
  if (bit_cnt) {
    write_buffer(outfile, bit_buf, bytes(bit_cnt));
  }

  return;
}

//
// Reads and returns next code in the input file.
// 4KB worth of codes are read into a code buffer.
// Called until main decompression loop decodes all characters.
//
// infile:     Input file descriptor.
// bit_len:    The length in bits of the code to read.
//
uint16_t next_code(int infile, uint8_t bit_len) {
  bit_total += bit_len;

  uint16_t code = 0;

  for (uint8_t i = 0; i < bit_len; i += 1) {
    if (!bit_cnt) {
      read_buffer(infile, bit_buf, BLOCK);
    }

    if (bit_buf[bit_cnt / BYTE] & (1 << (bit_cnt % BYTE))) {
      code |= (1 << (i % HALFWORD));
    } else {
      code &= ~(1 << (i % HALFWORD));
    }

    bit_cnt += 1;

    if (bit_cnt / BYTE == BLOCK) {
      bit_cnt = 0;
    }
  }

  return code;
}

//
// Adds a word into the character buffer to write to the output file.
// Buffer is written when it's filled with 4KB of characters.
//
// outfile:    Output file descriptor.
// word:       Word to output.
//
void buffer_word(int outfile, Word *w) {
  char_total += w->length;

  for (uint64_t i = 0; i < w->length; ++i) {
    char_buf[char_cnt++] = w->word[i];

    if (char_cnt == BLOCK) {
      write_buffer(outfile, char_buf, BLOCK);
      char_cnt = 0;
    }
  }

  return;
}

//
// Flushes the character buffer to the output file if not empty.
//
// outfile:    Output file descriptor.
//
void flush_words(int outfile) {
  if (char_cnt) {
    write_buffer(outfile, char_buf, char_cnt);
  }

  return;
}
