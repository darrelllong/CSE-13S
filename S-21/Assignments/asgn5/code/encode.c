#include "hamming.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "hi:o:"

static void usage(char *exec) {
    fprintf(stderr,
            "SYNOPSIS\n"
            "  A Hamming(8, 4) systematic code generator.\n"
            "\n"
            "USAGE\n"
            "  %s [-h] [-i infile] [-o outfile]\n"
            "\n"
            "OPTIONS\n"
            "  -h             Program usage and help.\n"
            "  -i infile      Input data to encode.\n"
            "  -o outfile     Output of encoded data.\n",
            exec);
}

static BitMatrix *generator(void) {
    BitMatrix *G = bm_create(4, 8);

    const char *matrix = "10000111"
                         "01001011"
                         "00101101"
                         "00011110";

    for (uint32_t r = 0; r < bm_rows(G); r += 1) {
        for (uint32_t c = 0; c < bm_cols(G); c += 1) {
            if (matrix[r * bm_cols(G) + c] == '1') {
                bm_set_bit(G, r, c);
            } else {
                bm_clr_bit(G, r, c);
            }
        }
    }

    return G;
}

int main(int argc, char **argv) {
    int opt;
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
    BitMatrix *G = generator();
    while ((c = fgetc(infile)) != EOF) {
        fputc(encode(G, c & 0x0F), outfile);
        fputc(encode(G, (c & 0xF0) >> 4), outfile);
    }

    bm_delete(&G);
    fclose(infile);
    fclose(outfile);
    return EXIT_SUCCESS;
}
