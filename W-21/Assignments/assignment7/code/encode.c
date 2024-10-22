#include "code.h"
#include "io.h"
#include "trie.h"
#include "util.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

#define OPTIONS "vhi:o:"

void usage(char *program) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "   Compresses files using the LZ78 compression algorithm.\n"
        "   Compressed files are decompressed with the corresponding decoder.\n"
        "\n"
        "USAGE\n"
        "   %s [-vh] [-i input] [-o output]\n"
        "\n"
        "OPTIONS\n"
        "   -v         Display compression statistics\n"
        "   -i input   Specify input to compress (default: stdin)\n"
        "   -o output  Specify output of compressed input (default: stdout)\n"
        "   -h         Display program help and usage\n",
        program);
}

void encode(int infile, int outfile) {
    TrieNode *root = trie_create();
    TrieNode *curr_node = root;
    TrieNode *prev_node = NULL;
    uint8_t curr_sym = 0;
    uint8_t prev_sym = 0;
    uint16_t next_code = START_CODE;

    while (read_sym(infile, &curr_sym)) {
        TrieNode *next_node = trie_step(curr_node, curr_sym);

        if (next_node) {
            prev_node = curr_node;
            curr_node = next_node;
        } else {
            write_pair(outfile, curr_node->code, curr_sym, bitwidth(next_code));
            curr_node->children[curr_sym] = trie_node_create(next_code++);
            curr_node = root;
        }

        if (next_code == MAX_CODE) {
            trie_reset(root);
            curr_node = root;
            next_code = START_CODE;
        }

        prev_sym = curr_sym;
    }

    if (curr_node != root) {
        write_pair(outfile, prev_node->code, prev_sym, bitwidth(next_code));
        next_code = (next_code + 1) % MAX_CODE;
    }

    write_pair(outfile, STOP_CODE, 0, bitwidth(next_code));
    flush_pairs(outfile);
    trie_delete(root);
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

    struct stat statbuf;
    fstat(infile, &statbuf);
    fchmod(outfile, statbuf.st_mode);

    FileHeader header = { 0, 0 };
    header.magic = MAGIC;
    header.protection = statbuf.st_mode;
    write_header(outfile, &header);

    encode(infile, outfile);

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
