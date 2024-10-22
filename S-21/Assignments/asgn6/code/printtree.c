#include "io.h"
#include "printer.h"
#include "header.h"
#include "huffman.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "hi:o:"

void usage(char *exec) {
    fprintf(stderr,
            "SYNOPSIS\n"
            "  Receives a Huffman header and tree.\n"
            "  Given a valid header and tree, pretty-prints the tree.\n"
            "\n"
            "USAGE\n"
            "  %s [-h] [-i infile] [-o outfile]\n"
            "\n"
            "OPTIONS\n"
            "  -h             Program usage and help.\n"
            "  -i infile      Input data to read header and tree from.\n"
            "  -o outfile     Output of pretty-printed tree.\n",
            exec);
}

int main(int argc, char **argv) {
    int opt;
    int infile  = STDIN_FILENO;
    int outfile = STDOUT_FILENO;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            if ((infile = open(optarg, O_RDONLY)) == -1) {
                fprintf(stderr, "Error: failed to open input file.\n");
                return EXIT_FAILURE;
            }
            break;
        case 'o':
            if ((outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) {
                fprintf(stderr, "Error: failed to open output file.\n");
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

    Header h;
    if (read_bytes(infile, (uint8_t *)&h, sizeof(Header)) != sizeof(Header)) {
        fprintf(stderr, "Error: unable to read header.\n");
        return EXIT_FAILURE;
    }
    if (h.magic != MAGIC) {
        fprintf(stderr, "Error: invalid magic number.\n");
        return EXIT_FAILURE;
    }

    fchmod(outfile, h.permissions);

    static uint8_t tree[MAX_TREE_SIZE];
    if (read_bytes(infile, tree, h.tree_size) < h.tree_size) {
        fprintf(stderr, "Error: invalid tree dump.\n");
        return EXIT_FAILURE;
    }

    Node *root = rebuild_tree(h.tree_size, tree);
    if (!root) {
        fprintf(stderr, "Error: invalid tree dump.\n");
        return EXIT_FAILURE;
    }

    binary_tree_print(root, node_print_sym, ALPHABET);

    delete_tree(&root);
    close(infile);
    close(outfile);
    return EXIT_SUCCESS;
}
