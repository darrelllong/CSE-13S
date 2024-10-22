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

void usage(char *program) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Decompresses files with the LZ78 decompression algorithm.\n"
        "   Used with files compressed with the corresponding encoder.\n"
        "\n"
        "USAGE\n"
        "   %s [-vh] [-i input] [-o output]\n"
        "\n"
        "OPTIONS\n"
        "   -v         Display decompression statistics\n"
        "   -i input   Specify input to decompress (default: stdin)\n"
        "   -o output  Specify output of decompressed input (default: stdout)\n"
        "   -h         Display program usage\n",
        program);
}

void decode(int infile, int outfile) {
    WordTable *wt = wt_create();
    uint8_t curr_sym = 0;
    uint16_t curr_code = 0;
    uint16_t next_code = START_CODE;

    while (read_pair(infile, &curr_code, &curr_sym, bitwidth(next_code))) {
        wt[next_code] = word_append_sym(wt[curr_code], curr_sym);
        write_word(outfile, wt[next_code++]);

        if (next_code == MAX_CODE) {
            wt_reset(wt);
            next_code = START_CODE;
        }
    }

    flush_words(outfile);
    wt_delete(wt);
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
            usage(argv[0]);
            return 0;
        default:
            usage(argv[0]);
            return 1;
        }
    }

    FileHeader header = { 0, 0 };
    read_header(infile, &header);
    check(header.magic == MAGIC, "Bad magic number.\n");
    fchmod(outfile, header.protection);

    decode(infile, outfile);

    if (verbose) {
        double ratio = 1.0 - (double)bytes(total_bits) / (double)total_syms;
        printf("Compressed file size: %" PRIu64 " bytes\n", bytes(total_bits));
        printf("Uncompressed file size: %" PRIu64 " bytes\n", total_syms);
        printf("Space saving: %.2lf%%\n", 100.0 * ratio);
    }

    close(infile);
    close(outfile);
    return 0;
}
