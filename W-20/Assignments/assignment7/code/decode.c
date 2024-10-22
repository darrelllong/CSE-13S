#include "code.h"
#include "io.h"
#include "util.h"
#include "word.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "vi:o:h"

//
// Prints out program usage.
//
// argv:    Arguments supplied to program (used for executable path).
// returns: Void.
//
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

//
// The LZ78 decompression algorithm.
// Decodes the input and dumps decoded data to the output.
//
// infile:  File descriptor of input the file to decode.
// outfile: File descriptor of output the file to dump decoded data to.
// returns: Void.
//
void decode(int infile, int outfile) {
  FileHeader header = { 0, 0 };
  read_header(infile, &header);
  check(header.magic == MAGIC, "Bad magic number.\n");
  fchmod(outfile, header.protection);

  WordTable *wt = wt_create();
  uint8_t curr_sym = 0;
  uint16_t curr_code = 0;
  uint16_t next_code = START_CODE;

  while (read_pair(infile, &curr_code, &curr_sym, bitwidth(next_code))) {
    wt[next_code] = word_append_sym(wt[curr_code], curr_sym);
    buffer_word(outfile, wt[next_code++]);

    if (next_code == MAX_CODE) {
      wt_reset(wt);
      next_code = START_CODE;
    }
  }

  flush_words(outfile);
  wt_delete(wt);
  return;
}

int main(int argc, char **argv) {
  int opt = 0;
  int infile = STDIN_FILENO;
  int outfile = STDOUT_FILENO;
  bool verbose = false;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'v':
      verbose = true;
      break;
    case 'i':
      infile = open(optarg, O_RDONLY);
      check(infile != -1, "Failed to open %s.\n", optarg);
      break;
    case 'o':
      outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC);
      check(outfile != -1, "Failed to open %s.\n", optarg);
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

  close(infile);
  close(outfile);

  return EXIT_SUCCESS;
}
