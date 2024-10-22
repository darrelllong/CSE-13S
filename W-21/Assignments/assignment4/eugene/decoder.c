#include "hamming.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "i:o:v"

int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    FILE *infile = stdin;
    FILE *outfile = stdout;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': { infile = fopen(optarg, "rb"); break; }
        case 'o': { outfile = fopen(optarg, "wb"); break; }
        case 'v': { verbose = true; break; }
        default:  { return 1; }
        }
    }

    struct stat statbuf;
    fstat(fileno(infile), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    int c = 0, status = 0;
    uint8_t nibble = 0, mibble = 0;
    uint64_t bytes = 0, corrected = 0, uncorrected = 0;

    while ((c = fgetc(infile)) != EOF) {
        status = decode(c, bytes & 1 ? &mibble : &nibble);
        if (status == -1 || status >= 0) {
            status == -1 ? (uncorrected += 1) : (corrected += 1);
        }
        if (bytes++ & 1) {
            fputc((mibble << 4) | nibble, outfile);
        }
    }


    if (verbose) {
        fprintf(stderr, "Total bytes processed: %" PRIu64 "\n", bytes);
        fprintf(stderr, "Uncorrected errors: %" PRIu64 "\n", uncorrected);
        fprintf(stderr, "Corrected errors: %" PRIu64 "\n", corrected);
        fprintf(stderr, "Error rate: %lf\n", (double) uncorrected / bytes);
    }

    fclose(infile);
    fclose(outfile);
    return 0;
}
