#include "hamming.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "hvi:o:"

static void usage(char *exec) {
    fprintf(stderr,
            "SYNOPSIS\n"
            "  A Hamming(8, 4) systematic code decoder.\n"
            "\n"
            "USAGE\n"
            "  %s [-h] [-v] [-i infile] [-o outfile]\n"
            "\n"
            "OPTIONS\n"
            "  -h             Program usage and help.\n"
            "  -v             Print statistics of decoding to stderr.\n"
            "  -i infile      Input data to decode.\n"
            "  -o outfile     Output of decoded data.\n",
            exec);
}

static BitMatrix *parity(void) {
    BitMatrix *Ht = bm_create(8, 4);

    const char *matrix = "0111"
                         "1011"
                         "1101"
                         "1110"
                         "1000"
                         "0100"
                         "0010"
                         "0001";

    for (uint32_t r = 0; r < bm_rows(Ht); r += 1) {
        for (uint32_t c = 0; c < bm_cols(Ht); c += 1) {
            if (matrix[r * bm_cols(Ht) + c] == '1') {
                bm_set_bit(Ht, r, c);
            } else {
                bm_clr_bit(Ht, r, c);
            }
        }
    }

    return Ht;
}

int main(int argc, char **argv) {
    int opt;
    bool verbose = false;
    FILE *infile = stdin, *outfile = stdout;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            infile = fopen(optarg, "rb");
            if (!infile) {
                fprintf(stderr, "Error: failed to open %s.\n", optarg);
                return EXIT_FAILURE;
            }
            break;
        case 'o':
            outfile = fopen(optarg, "wb");
            if (!outfile) {
                fprintf(stderr, "Error: failed to open %s.\n", optarg);
                return EXIT_FAILURE;
            }
            break;
        case 'v':
            verbose = true;
            break;
        case 'h':
            usage(argv[0]);
            return EXIT_SUCCESS;
        default:
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    struct stat statbuf;
    fstat(fileno(infile), &statbuf);
    fchmod(fileno(outfile), statbuf.st_mode);

    int c;
    BitMatrix *Ht = parity();
    uint8_t lower = 0, upper = 0;
    uint64_t bytes = 0, corrected = 0, uncorrected = 0;

    while ((c = fgetc(infile)) != EOF) {
        switch (decode(Ht, (uint8_t)c, bytes & 1 ? &upper : &lower)) {
        case HAM_CORRECT:
            corrected += 1;
            break;
        case HAM_ERR:
            uncorrected += 1;
            break;
        default:
            break;
        }
        if (bytes & 1) {
            fputc((upper << 4) | lower, outfile);
        }
        bytes += 1;
    }

    if (verbose) {
        fprintf(stderr, "Total bytes processed: %" PRIu64 "\n", bytes);
        fprintf(stderr, "Uncorrected errors: %" PRIu64 "\n", uncorrected);
        fprintf(stderr, "Corrected errors: %" PRIu64 "\n", corrected);
        fprintf(stderr, "Error rate: %lf\n", (double)uncorrected / bytes);
    }

    bm_delete(&Ht);
    fclose(infile);
    fclose(outfile);
    return EXIT_SUCCESS;
}
