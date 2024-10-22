#include "io.h"
#include "trie.h"
#include "util.h"
#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "vhi:o:"

void print_usage(char **argv) {
  fprintf(stderr,
    "SYNOPSIS\n"
    "   Compresses files using the LZ78 compression algorithm.\n"
    "   Compressed files are decompressed with the corresponding decoder.\n"
    "\n"
    "USAGE\n"
    "   %s [-vh] [-i input] [-o output]\n"
    "\n"
    "OPTIONS\n"
    "   -v          Display compression statistics\n"
    "   -i input    Specify input to compress (stdin by default)\n"
    "   -o output   Specify output of compressed input (stdout by default)\n"
    "   -h          Display program help and usage\n",
    argv[0]);
  return;
}

void encode(FILE *infile, FILE *outfile) {
  struct stat statbuf;
  fstat(fileno(infile), &statbuf);
  fchmod(fileno(outfile), statbuf.st_mode);

  FileHeader header = { 0, 0 };
  header.magic = MAGIC;
  header.protection = statbuf.st_mode;
  write_header(outfile, &header);

  trie_init();
  uint8_t prev_sym = 0;
  uint8_t curr_sym = 0;
  uint16_t temp_code = 0;
  uint16_t prev_code = 0;
  uint16_t curr_code = EMPTY_CODE;
  uint16_t next_code = START_CODE;

  while (read_sym(infile, &curr_sym)) {
    if ((temp_code = trie_step(curr_code, curr_sym))) {
      prev_code = curr_code;
      curr_code = temp_code;
    } else {
      buffer_pair(outfile, curr_code, curr_sym, bitwidth(next_code));
      trie_add(curr_code, curr_sym, next_code++);
      curr_code = EMPTY_CODE;
    }

    if (next_code == MAX_CODE) {
      trie_init();
      curr_code = EMPTY_CODE;
      next_code = START_CODE;
    }

    prev_sym = curr_sym;
  }

  if (curr_code != EMPTY_CODE) {
    buffer_pair(outfile, prev_code, prev_sym, bitwidth(next_code++));
    next_code = (next_code == MAX_CODE) ? START_CODE : next_code;
  }

  buffer_pair(outfile, STOP_CODE, 0, bitwidth(next_code));
  flush_pairs(outfile);
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

  encode(infile, outfile);

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
