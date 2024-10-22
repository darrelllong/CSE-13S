#include "endian.h"
#include "io.h"
#include "util.h"
#include "word.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

uint64_t total_syms = 0;
uint64_t total_bits = 0;

static uint8_t sym_buf[BLOCK] = { 0 };
static uint16_t sym_cnt = 0;

static uint8_t bit_buf[BLOCK] = { 0 };
static uint16_t bit_cnt = 0;

void read_header(FILE *infile, FileHeader *header) {
  fread(header, sizeof(FileHeader), 1, infile);

  if (big_endian()) {
    header->magic = swap32(header->magic);
    header->protection = swap16(header->protection);
  }

  total_bits += (BYTE * sizeof(FileHeader));
  return;
}

void write_header(FILE *outfile, FileHeader *header) {
  total_bits += (BYTE * sizeof(FileHeader));

  if (big_endian()) {
    header->magic = swap32(header->magic);
    header->protection = swap16(header->protection);
  }

  fwrite(header, sizeof(FileHeader), 1, outfile);
  return;
}

bool read_sym(FILE *infile, uint8_t *sym) {
  // Tracks the end of the symbol buffer.
  // We don't keep track of the end until less than a block is read in.
  static uint16_t sym_buf_end = UINT16_MAX;

  // Buffer another block if the buffer is fully processed.
  // If less than a block is read, update the end of the buffer.
  if (!sym_cnt) {
    int bytes_read = fread(sym_buf, sizeof(uint8_t), BLOCK, infile);
    sym_buf_end = (bytes_read == BLOCK) ? sym_buf_end : bytes_read + 1;
  }

  // Store next symbol, update total symbol count.
  *sym = sym_buf[sym_cnt];
  sym_cnt = (sym_cnt + 1) % BLOCK;
  total_syms = (sym_cnt != sym_buf_end) ? total_syms + 1 : total_syms;

  return sym_cnt != sym_buf_end;
}

void buffer_pair(FILE *outfile, uint16_t code, uint8_t sym, uint8_t bit_len) {
  total_bits += BYTE + bit_len;

  if (big_endian()) {
    code = swap16(code);
  }

  // Buffer each bit of the code.
  // Write out buffer if filled and zero it out.
  for (uint8_t i = 0; i < bit_len; i += 1) {
    if (code & (1 << (i % SHORT))) {
      bit_buf[bit_cnt / BYTE] |= (1 << (bit_cnt % BYTE));
    }

    bit_cnt += 1;

    if (bit_cnt / BYTE == BLOCK) {
      fwrite(bit_buf, sizeof(uint8_t), BLOCK, outfile);
      memset(bit_buf, 0, BLOCK);
      bit_cnt = 0;
    }
  }

  // Buffer each bit of the symbol
  // Write out buffer if filled and zero it out.
  for (uint8_t i = 0; i < BYTE; i += 1) {
    if (sym & (1 << (i % BYTE))) {
      bit_buf[bit_cnt / BYTE] |= (1 << (bit_cnt % BYTE));
    }

    bit_cnt += 1;

    if (bit_cnt / BYTE == BLOCK) {
      fwrite(bit_buf, sizeof(uint8_t), BLOCK, outfile);
      memset(bit_buf, 0, BLOCK);
      bit_cnt = 0;
    }
  }

  return;
}

void flush_pairs(FILE *outfile) {
  // Flush any remaining bits, if any, left in the buffer.
  if (bit_cnt) {
    fwrite(bit_buf, sizeof(uint8_t), bytes(bit_cnt), outfile);
  }

  return;
}

bool read_pair(FILE *infile, uint16_t *code, uint8_t *sym, uint8_t bit_len) {
  total_bits += BYTE + bit_len;
  *code = 0;  // May contain left over bits; init to zero to be safe.
  *sym = 0;   // May contain left over bits; init to zero to be safe.

  for (uint8_t i = 0; i < bit_len; i += 1) {
    // Refill buffer with bits from input.
    if (!bit_cnt) {
      fread(bit_buf, sizeof(uint8_t), BLOCK, infile);
    }

    // Add each bit of the code from the buffer.
    if (bit_buf[bit_cnt / BYTE] & (1 << (bit_cnt % BYTE))) {
      *code |= (1 << (i % SHORT));
    }

    bit_cnt = (bit_cnt + 1) % (BLOCK * BYTE);
  }

  for (uint8_t i = 0; i < BYTE; i += 1) {
    // Refill buffer with bits from input.
    if (!bit_cnt) {
      fread(bit_buf, sizeof(uint8_t), BLOCK, infile);
    }

    // Add each bit of the symbol from the buffer.
    if (bit_buf[bit_cnt / BYTE] & (1 << (bit_cnt % BYTE))) {
      *sym |= (1 << (i % BYTE));
    }

    bit_cnt = (bit_cnt + 1) % (BLOCK * BYTE);
  }

  if (big_endian()) {
    *code = swap16(*code);
  }

  return *code != STOP_CODE;
}

void buffer_word(FILE *outfile, uint16_t code, uint8_t sym) {
  // The count is the count of symbols in the word the code denotes.
  // Chaining the codes prepares the stack of symbols in word.c.
  uint32_t count = chain_codes(code);
  total_syms += count + 1;

  // Buffer each symbol.
  // The last symbol to buffer is the symbol to append.
  for (uint32_t i = 0; i <= count; i += 1) {
    if (i == count) {
      sym_buf[sym_cnt++] = sym;
    } else {
      sym_buf[sym_cnt++] = get_sym();
    }

    // Write buffer if filled.
    if (sym_cnt == BLOCK) {
      fwrite(sym_buf, sizeof(uint8_t), BLOCK, outfile);
      sym_cnt = 0;
    }
  }

  return;
}

void flush_words(FILE *outfile) {
  // Flush remaining bytes, if any, to the output.
  if (sym_cnt) {
    fwrite(sym_buf, sizeof(uint8_t), sym_cnt, outfile);
  }

  return;
}
