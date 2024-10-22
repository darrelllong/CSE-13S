#ifndef __IO_H__
#define __IO_H__

#include "word.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>

extern uint64_t total_syms;
extern uint64_t total_bits;

typedef struct FileHeader {
  uint32_t magic;
  uint16_t protection;
} FileHeader;

int read_bytes(FILE *infile, uint8_t *buf, int to_read);

int write_bytes(FILE *outfile, uint8_t *buf, int to_write);

void read_header(FILE *infile, FileHeader *header);

void write_header(FILE *outfile, FileHeader *header);

bool read_sym(FILE *infile, uint8_t *byte);

void buffer_pair(FILE *outfile, uint16_t code, uint8_t sym, uint8_t bit_len);

void flush_pairs(FILE *outfile);

bool read_pair(FILE *infile, uint16_t *code, uint8_t *sym, uint8_t bit_len);

void buffer_word(FILE *outfile, uint16_t code, uint8_t sym);

void flush_words(FILE *outfile);

#endif

