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

#define OPTIONS   "hvcdi:o:b:"
#define BITWIDTH  16

typedef enum { NOTHING, COMPRESS, DECOMPRESS } ops;

extern uint64_t bit_total;
extern uint64_t char_total;
extern int exit_status;

uint64_t max_word_len = 0;
uint8_t bitwidth = BITWIDTH;

//
// Prints program usage.
//
// argv:   Command-line arguments supplied to program.
//
void print_usage(char **argv) {
  bool encode = strcmp(basename(argv[0]), "encode") == 0;
  bool decode = strcmp(basename(argv[0]), "decode") == 0;

  if (encode) {
    errprint("Encodes an input to output using the LZW algorithm\n"
             "Usage: %s [-bv] -i <input> -o <output>\n", argv[0]);
  } else if (decode) {
    errprint("Decodes an input to output using the LZW algorithm\n"
             "Usage: %s [-bv] -i <input> -o <output>\n", argv[0]);
  } else {
    errprint("Encodes or decodes an input to output using the LZW algorithm\n"
             "Usage: %s [-bvcd] -i <input> -o <output>\n", argv[0]);
  }

  errprint("Options:\n"
           "    -h: Display program usage\n"
           "    -b: Specify bitwidth of codes (compression only)\n"
           "    -v: Enable verbose flag to print statistics\n"
           "    -i: Specify an input file to use\n"
           "    -o: Specify an output file to use\n");

  if (!encode && !decode) {
    errprint("    -c: Enable compression mode\n"
             "    -d: Enable decompression mode\n");
  }

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
    case 'v':   { verbose = true;         break; }
    case 'c':   { operation = COMPRESS;   break; }
    case 'd':   { operation = DECOMPRESS; break; }
    case 'i':   { infname = optarg;       break; }
    case 'o':   { outfname = optarg;      break; }
    case 'b':
      bitwidth = (uint8_t)strtoull(optarg, NULL, 10);
      ASSERT(bitwidth > 8, "%s\n", "Bitwidth must be greater than 8");
      ASSERT(bitwidth <= 16, "%s\n", "Bitwidth can't be greater than 16");
      break;
    case 'h':   { print_usage(argv); exit(exit_status); }
    default:    { print_usage(argv); exit(exit_status); }
    }
  }

  // Linked compress?
  if (strcmp(basename(argv[0]), "encode") == 0) {
    operation = COMPRESS;
  }

  // Linked decompress?
  if (strcmp(basename(argv[0]), "decode") == 0) {
    operation = DECOMPRESS;
  }

  // Handle input file (stdin defaulted if not specified).
  int infile = 0;
  if (!infname) {
    infile = isatty(STDIN_FILENO) ? mkstemp_stdin() : STDIN_FILENO;
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
  case COMPRESS:    { encode(infile, outfile); break; }
  case DECOMPRESS:  { decode(infile, outfile); break; }
  case NOTHING:     { print_usage(argv); exit(exit_status); }
  default:          { break; }
  }

  // Print statistics if verbose flag was set.
  if (verbose) {
    uint64_t orig_size = char_total;
    uint64_t zip_size = bytes(bit_total) + sizeof(FileHeader);
    float ratio = 1.0 - (float)zip_size / (float)orig_size;

    printf("Original file size: %" PRId64 " bytes\n", orig_size);
    printf("Compressed file size: %" PRId64 " bytes\n", zip_size);
    printf("Compression ratio: %.2lf%%\n", 100.0 * ratio);
    printf("Bitwidth: %" PRIu8 "\n", bitwidth);
    printf("Max word length: %" PRId64 " bytes\n", max_word_len);
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
