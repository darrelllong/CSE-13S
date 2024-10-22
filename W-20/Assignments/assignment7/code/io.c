#include "code.h"
#include "endian.h"
#include "io.h"
#include "util.h"
#include "word.h"

#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

uint64_t total_syms = 0;  // Total number of symbols processed.
uint64_t total_bits = 0;  // Total number of bits processed.

static uint8_t sym_buf[BLOCK] = { 0 };      // Buffer to store symbols in.
static uint16_t sym_cnt = 0;                // Index of current buffered symbol.
static uint16_t sym_buf_end = UINT16_MAX;   // Index of the end of the buffer.

static uint8_t bit_buf[BLOCK] = { 0 };      // Buffer to store bits in.
static uint16_t bit_cnt = 0;                // Index of current buffered bit.

//
// Wrapper for the read() syscall.
// Loops to read the specified number of bytes, or until input is exhausted.
// Returns the number of bytes read.
//
// infile:  File descriptor of the input file to read from.
// buf:     Buffer to store read bytes into.
// to_read: Number of bytes to read.
// returns: Number of bytes read.
//
int read_bytes(int infile, uint8_t *buf, int to_read) {
  int bytes_to_read = to_read;
  int bytes_read = 0;
  int total_read = 0;

  do {
    bytes_read = read(infile, buf + total_read, bytes_to_read);
    check(bytes_read != -1, "Failed to read from input file.\n");
    bytes_to_read -= bytes_read;
    total_read += bytes_read;
  } while (bytes_read && total_read != to_read);

  return total_read;
}

//
// Wrapper for the write() syscall.
// Loops to write the specified number of bytes, or until nothing is written.
// Returns the number of bytes written.
//
// outfile:   File descriptor of the output file to write to.
// buf:       Buffer that stores the bytes to write out.
// to_write:  Number of bytes to write.
// returns:   Number of bytes written.
//
int write_bytes(int outfile, uint8_t *buf, int to_write) {
  int bytes_to_write = to_write;
  int bytes_written = 0;
  int total_written = 0;

  do {
    bytes_written = write(outfile, buf + total_written, bytes_to_write);
    check(bytes_written != -1, "Failed to write to output file.\n");
    bytes_to_write -= bytes_written;
    total_written += bytes_written;
  } while (bytes_written && total_written != to_write);

  return total_written;
}

//
// Reads in a FileHeader from the input file.
// Endianness of header fields are swapped if byte order isn't little endian.
//
// infile:  File descriptor of input file to read header from.
// header:  Pointer to memory where the bytes of the read header should go.
// returns: Void.
//
void read_header(int infile, FileHeader *header) {
  read_bytes(infile, (uint8_t *) header, sizeof(FileHeader));

  if (big_endian()) {
    header->magic = swap32(header->magic);
    header->protection = swap16(header->protection);
  }

  total_bits += (BYTE * sizeof(FileHeader));
  return;
}

//
// Writes a FileHeader to the output file.
// Endianness of header fields are swapped if byte order isn't little endian.
//
// outfile: File descriptor of output file to write header to.
// header:  Pointer to the header to write out.
// returns: Void.
//
void write_header(int outfile, FileHeader *header) {
  total_bits += (BYTE * sizeof(FileHeader));

  if (big_endian()) {
    header->magic = swap32(header->magic);
    header->protection = swap16(header->protection);
  }

  write_bytes(outfile, (uint8_t *) header, sizeof(FileHeader));
  return;
}

//
// "Reads" a symbol from the input file.
// The "read" symbol is placed into the pointer to sym (pass by reference).
// In reality, a block of symbols is read into a buffer.
// An index keeps track of the currently read symbol in the buffer.
// Once all symbols are processed, another block is read.
// If less than a block is read, the end of the buffer is updated.
// Returns true if there are symbols to be read, false otherwise.
//
// infile:  File descriptor of input file to read symbols from.
// sym:     Pointer to memory which stores the read symbol.
// returns: True if there are symbols to be read, false otherwise.
//
bool read_sym(int infile, uint8_t *sym) {
  if (!sym_cnt) {
    int bytes_read = read_bytes(infile, sym_buf, BLOCK);
    sym_buf_end = (bytes_read == BLOCK) ? sym_buf_end : bytes_read + 1;
  }

  *sym = sym_buf[sym_cnt];
  sym_cnt = (sym_cnt + 1) % BLOCK;
  total_syms = (sym_cnt != sym_buf_end) ? total_syms + 1 : total_syms;

  return sym_cnt != sym_buf_end;
}

//
// Buffers a pair. A pair is comprised of a symbol and an index.
// The bits of the symbol are buffered first, starting from the LSB.
// The bits of the index are buffered next, also starting from the LSB.
// bit_len bits of the index are buffered to provide a minimal representation.
// The buffer is written out whenever it is filled.
//
// outfile: File descriptor of the output file to write to.
// sym:     Symbol of the pair to buffer.
// index:   Index of the pair to buffer.
// bit_len: Number of bits of the index to buffer.
// returns: Void.
//
void buffer_pair(int outfile, uint16_t code, uint8_t sym, uint8_t bit_len) {
  total_bits += BYTE + bit_len;

  if (big_endian()) {
    code = swap16(code);
  }

  for (uint8_t i = 0; i < bit_len; i += 1) {
    if (code & (1 << (i % SHORT))) {
      bit_buf[bit_cnt / BYTE] |= (1 << (bit_cnt % BYTE));
    } else {
      bit_buf[bit_cnt / BYTE] &= ~(1 << (bit_cnt % BYTE));
    }

    bit_cnt += 1;

    if (bit_cnt / BYTE == BLOCK) {
      write_bytes(outfile, bit_buf, BLOCK);
      bit_cnt = 0;
    }
  }

  for (uint8_t i = 0; i < BYTE; i += 1) {
    if (sym & (1 << (i % BYTE))) {
      bit_buf[bit_cnt / BYTE] |= (1 << (bit_cnt % BYTE));
    } else {
      bit_buf[bit_cnt / BYTE] &= ~(1 << (bit_cnt % BYTE));
    }

    bit_cnt += 1;

    if (bit_cnt / BYTE == BLOCK) {
      write_bytes(outfile, bit_buf, BLOCK);
      bit_cnt = 0;
    }
  }

  return;
}

//
// Writes out any remaining pairs of symbols and indexes to the output file.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
//
void flush_pairs(int outfile) {
  if (bit_cnt) {
    write_bytes(outfile, bit_buf, bytes(bit_cnt));
  }

  return;
}

//
// "Reads" a pair (symbol and index) from the input file.
// The "read" symbol is placed in the pointer to sym (pass by reference).
// The "read" index is placed in the pointer to index (pass by reference).
// In reality, a block of pairs is read into a buffer.
// An index keeps track of the current bit in the buffer.
// Once all bits have been processed, another block is read.
// The first 8 bits of the pair constitute the symbol, starting from the LSB.
// The next bit_len bits constitutes the index, starting from the the LSB.
// Returns true if there are pairs left to read in the buffer, else false.
// There are pairs left to read if the read index is not STOP_CODE.
//
// infile:  File descriptor of the input file to read from.
// sym:     Pointer to memory which stores the read symbol.
// index:   Pointer to memory which stores the read index.
// bit_len: Length in bits of the index to read.
// returns: True if there are pairs left to read, false otherwise.
//
bool read_pair(int infile, uint16_t *code, uint8_t *sym, uint8_t bit_len) {
  total_bits += BYTE + bit_len;
  *code = 0;
  *sym = 0;

  for (uint8_t i = 0; i < bit_len; i += 1) {
    if (!bit_cnt) {
      read_bytes(infile, bit_buf, BLOCK);
    }

    if (bit_buf[bit_cnt / BYTE] & (1 << (bit_cnt % BYTE))) {
      *code |= (1 << (i % SHORT));
    } else {
      *code &= ~(1 << (i % SHORT));
    }

    bit_cnt = (bit_cnt + 1) % (BLOCK * BYTE);
  }

  for (uint8_t i = 0; i < BYTE; i += 1) {
    if (!bit_cnt) {
      read_bytes(infile, bit_buf, BLOCK);
    }

    if (bit_buf[bit_cnt / BYTE] & (1 << (bit_cnt % BYTE))) {
      *sym |= (1 << (i % BYTE));
    } else {
      *sym &= ~(1 << (i % BYTE));
    }

    bit_cnt = (bit_cnt + 1) % (BLOCK * BYTE);
  }

  if (big_endian()) {
    *code = swap16(*code);
  }

  return *code != STOP_CODE;
}

//
// Buffers a Word, or more specifically, the symbols of a Word.
// Each symbol of the Word is placed into a buffer.
// The buffer is written out when it is filled.
//
// outfile: File descriptor of the output file to write to.
// w:       Word to buffer.
// returns: Void.
//
void buffer_word(int outfile, Word *w) {
  total_syms += w->len;

  for (uint32_t i = 0; i < w->len; i += 1) {
    sym_buf[sym_cnt++] = w->syms[i];

    if (sym_cnt == BLOCK) {
      write_bytes(outfile, sym_buf, BLOCK);
      sym_cnt = 0;
    }
  }

  return;
}

//
// Writes out any remaining symbols in the buffer.
//
// outfile: File descriptor of the output file to write to.
// returns: Void.
//
void flush_words(int outfile) {
  if (sym_cnt) {
    write_bytes(outfile, sym_buf, sym_cnt);
  }

  return;
}
