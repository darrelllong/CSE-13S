#include "io.h"
#include "util.h"
#include "word.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "vi:o:h"

void print_usage(char **argv) {
  fprintf(stderr,
    "SYNOPSIS\n"
    "   Decompresses files with the LZ78 decompression algorithm.\n"
    "   Used with files compressed with the corresponding encoder.\n"
    "\n"
    "USAGE\n"
    "   %s [-vh] [-i input] [-o output]\n"
    "\n"
    "OPTIONS\n"
    "   -v          Display decompression statistics\n"
    "   -i input    Specify input to decompress (stdin by default)\n"
    "   -o output   Specify output of decompressed input (stdout by default)\n"
    "   -h          Display program usage\n",
    argv[0]);
  return;
}

void decode(FILE *infile, FILE *outfile) {
  FileHeader header = { 0, 0 };
  read_header(infile, &header);
  check(header.magic == MAGIC, "Bad magic number.\n");
  fchmod(fileno(outfile), header.protection);

  words_init();
  uint8_t curr_sym = 0;
  uint16_t curr_code = 0;
  uint16_t next_code = START_CODE;

  while (read_pair(infile, &curr_code, &curr_sym, bitwidth(next_code))) {
    words_add(curr_code, curr_sym, next_code++);
    buffer_word(outfile, curr_code, curr_sym);

    if (next_code == MAX_CODE) {
      words_init();
      next_code = START_CODE;
    }
  }

  flush_words(outfile);
  return;
}

int main(int argc, char **argv) {
  int opt = 0;
  FILE *infile = stdin;
  FILE *outfile = stdout;
  bool verbose = false;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'v':
      verbose = true;
      break;
    case 'i':
      infile = fopen(optarg, "rb");
      check(infile, "Failed to open %s.\n", optarg);
      break;
    case 'o':
      outfile = fopen(optarg, "wb");
      check(outfile, "Failed to open %s.\n", optarg);
      break;
    case 'h':
      print_usage(argv);
      exit(EXIT_SUCCESS);
    default:
      print_usage(argv);
      exit(EXIT_FAILURE);
    }
  }

  decode(infile, outfile);

  if (verbose) {
    double ratio = 1.0 - (double) bytes(total_bits) / (double) total_syms;
    printf("Compressed file size: %" PRIu64 " bytes\n", bytes(total_bits));
    printf("Uncompressed file size: %" PRIu64 " bytes\n", total_syms);
    printf("Compression ratio: %.2lf%%\n", 100.0 * ratio);
  }

  fclose(infile);
  fclose(outfile);

  return EXIT_SUCCESS;
}
