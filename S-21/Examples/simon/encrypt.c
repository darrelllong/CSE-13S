#include "simon.h"
#include "util.h"
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

#define OPTIONS "hvsk:i:o:"

void print_usage(char **argv) {
  fprintf(stderr,
    "SYNOPSIS\n"
    "   Encrypts blocks of data using the Simon 128/256 block cipher.\n"
    "   Data is encrypted using a 256-bit key stored in a file.\n"
    "   Data to encrypt must be 126 bits (two 64-bit words).\n"
    "   Encrypted data is decrypted with corresponding decrypt program.\n"
    "\n"
    "USAGE\n"
    "   %s [-hvs] -k key [-i input] [-o output]\n"
    "\n"
    "OPTIONS\n"
    "   -h          Display program help and usage\n"
    "   -v          Display verbose program output\n"
    "   -s          Encrypt using Simple Simon 128/256\n"
    "   -k key      Specify key to encrypt with (mandatory)\n"
    "   -i input    Specify input to encrypt (stdin by default)\n"
    "   -o output   Specify output of encrypted input (stdout by default)\n",
    argv[0]);
  return;
}

void encryption(FILE *infile, FILE *outfile, FILE *keyfile) {
  uint8_t k[32] = { 0 };
  fread(k, sizeof(uint8_t), 32, keyfile);

  uint64_t K[4] = { 0 };
  bytes_to_words(k, K, 32);

  uint8_t pt[16] = { 0 };
  fread(pt, sizeof(uint8_t), 16, infile);

  uint64_t Pt[2] = { 0 };
  bytes_to_words(pt, Pt, 16);

  simon_key_schedule(K);

  uint64_t Ct[2] = { 0 };
  simon_encrypt(Pt, Ct);

  uint8_t ct[16] = { 0 };
  words_to_bytes(Ct, ct, 2);

  fwrite(ct, sizeof(uint8_t), 16, outfile);

  if (verbose) {
    output("K = ");
    print_hexstring(k, 32);

    output("Pt = ");
    print_hexstring(pt, 16);

    output("Ct = ");
    print_hexstring(ct, 16);
  }

  return;
}

int main(int argc, char **argv) {
  int opt = 0;
  FILE *keyfile = NULL;
  FILE *infile = stdin;
  FILE *outfile = stdout;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'h':
      print_usage(argv);
      return 0;
    case 'v':
      verbose = true;
      break;
    case 's':
      simple = true;
      break;
    case 'k':
      keyfile = fopen(optarg, "rb");
      check(keyfile, "Failed to open %s.\n", optarg);
      break;
    case 'i':
      infile = fopen(optarg, "rb");
      check(infile, "Failed to open %s.\n", optarg);
      break;
    case 'o':
      outfile = fopen(optarg, "wb");
      check(outfile, "Failed to open %s.\n", optarg);
      break;
    default:
      print_usage(argv);
      return 1;
    }
  }

  check(keyfile, "A file containing the key must be supplied!\n");

  encryption(infile, outfile, keyfile);

  fclose(keyfile);
  fclose(infile);
  fclose(outfile);

  return 0;
}
