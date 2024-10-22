#include "io.h"
#include "util.h"
#include "encode.h"
#include "decode.h"
#include <fcntl.h>
#include <getopt.h>
#include <inttypes.h>
#include <libgen.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "vcdi:o:"

typedef enum { NOTHING, COMPRESS, DECOMPRESS } ops;

extern uint64_t bit_total;
extern uint64_t char_total;
extern int exit_status;
uint64_t max_word_len;

//
// Prints program usage.
//
// argv:   Command-line arguments supplied to program.
//
void print_usage(char **argv) {
  errprint("Usage: %s -[pcd] -i <input> -o <output>\n"
           "Options:\n"
           "    -v: Enable verbose flag to print statistics\n"
           "    -c: Enable compression flag\n"
           "    -d: Enable decompression flag\n"
           "    -i: Specify an input file to use\n"
           "    -o: Specify an output file to use\n",
           argv[0]);
  return;
}

int main(int argc, char **argv) {
  int opt = 0;
  bool verbose = false;
  char *infname = NULL;
  char *outfname = NULL;
  ops operation = NOTHING;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'v':
      verbose = true;
      break;
    case 'c':
      operation = COMPRESS;
      break;
    case 'd':
      operation = DECOMPRESS;
      break;
    case 'i':
      infname = optarg;
      break;
    case 'o':
      outfname = optarg;
      break;
    default:
      print_usage(argv);
      exit(exit_status);
    }
  }

  // Linked compress?
  if (strncmp(basename(argv[0]), "encode", strlen("encode")) == 0) {
    operation = COMPRESS;
  }

  // Linked decompress?
  if (strncmp(basename(argv[0]), "decode", strlen("decode")) == 0) {
    operation = DECOMPRESS;
  }

  // Handle input file (stdin defaulted if not specified).
  int infile = 0;
  if (!infname) {
    if (isatty(STDIN_FILENO)) {
      infile = mkstemp_stdin(false);
    } else if (ispipe(STDIN_FILENO)) {
      infile = mkstemp_stdin(true);
    } else {
      infile = STDIN_FILENO;
    }
  } else {
    infile = open(infname, O_RDONLY);
    ASSERT(infile != -1, "Failed to open %s\n", infname);
  }

  // Handle output file (stdout defaulted if not specified).
  int outfile = 0;
  if (!outfname) {
    outfile = STDOUT_FILENO;
  } else {
    outfile = open(outfname, O_WRONLY | O_CREAT | O_TRUNC);
    ASSERT(outfile != -1, "Failed to open %s\n", outfname);
  }

  // Compress or decompress?
  switch (operation) {
  case COMPRESS:
    encode(infile, outfile);
    break;
  case DECOMPRESS:
    decode(infile, outfile);
    break;
  case NOTHING:
    print_usage(argv);
    exit(exit_status);
  }

  // Print statistics if verbose flag was set.
  if (verbose) {
    uint64_t orig_size = char_total;
    uint64_t zip_size = bytes(bit_total) + sizeof(FileHeader);
    float ratio = 1.0 - (float)zip_size / (float)orig_size;

    printf("Original file size: %" PRId64 "\n", orig_size);
    printf("Compressed file size: %" PRId64 "\n", zip_size);
    printf("Compression ratio: %lf%%\n", 100.0 * ratio);
    printf("Longest word length: %" PRId64 "\n", max_word_len);
  }

  // Close input file if not stdin.
  if (infile != STDIN_FILENO) {
    close(infile);
  }

  // Close output file if not stdout.
  if (outfile != STDOUT_FILENO) {
    close(outfile);
  }

  return exit_status;
}
