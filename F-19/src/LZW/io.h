#ifndef __IO_H__
#define __IO_H__

#include "word.h"
#include <inttypes.h>

// Variables to keep track of statistics for verbose flag.
extern uint64_t char_total;
extern uint64_t bit_total;

//
// Struct definition for a compressed file's header.
//
// magic:       Magic number that verifies this compression program.
// file_size:   File size of original, uncompressed file.
// protection:  The protection bit mask.
// bitwidth:    The maximum length in bits of a code (max is 16 bits).
// padding:     Padding to align struct to 32-bits.
//
typedef struct FileHeader {
  uint32_t magic;
  uint64_t file_size;
  uint16_t protection;
  uint8_t bitwidth;
  uint8_t padding;
} FileHeader;

//
// Reads a file header from the input file.
//
// infile:     Input file descriptor.
// header:     The file header struct to read into.
//
void read_header(int infile, FileHeader *header);

//
// Writes a file header to the output file and returns original file size.
//
// outfile:    Output file descriptor.
// header:     FileHeader to set values in and write.
//
void write_header(int outfile, FileHeader *header);

//
// Returns next character, or byte, from the input file.
// 4KB of characters are read when needed into a character buffer.
// This is only called once for each byte in the input file.
//
// infile:     Input file descriptor
//
uint8_t next_char(int infile);

//
// Buffers a code into the code buffer for writing.
// The binary of the code is placed into the buffer in reverse.
//
// outfile:   Output file descriptor.
// code:      The code to buffer.
// bit_len:   The length in bits of the code.
//
void buffer_code(int outfile, uint16_t code, uint8_t bit_len);

//
// Flushes any remaining codes in the buffer to the output file.
//
// outfile:    Output file descriptor.
//
void flush_code(int outfile);

//
// Reads and returns next code in the input file.
// 4KB worth of codes are read into a code buffer.
// Called until main decompression loop decodes all characters.
//
// infile:     Input file descriptor.
// bit_len:    The length in bits of the code to read.
//
uint16_t next_code(int infile, uint8_t bit_len);

//
// Adds a word into the character buffer to write to the output file.
// Buffer is written when it's filled with 4KB of characters.
//
// outfile:    Output file descriptor.
// word:       Word to output.
// word_len:   Length of word to output.
//
void buffer_word(int outfile, Word *w);

//
// Flushes the character buffer to the output file if not empty.
//
// outfile:    Output file descriptor.
//
void flush_word(int outfile);

#endif
