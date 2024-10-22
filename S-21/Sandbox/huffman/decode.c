#include "huffman.h"
#include "io.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OPTIONS "vti:o:"

void write_symbols(int infile, int outfile, Node *root, uint64_t file_size) {
    int byte = 0;
    uint8_t bit = 0;
    Node *curr = root;
    uint8_t buffer[BLOCK];

    while (file_size > 0 && read_bit(infile, &bit)) {
        curr = bit ? curr->right : curr->left;
        if (!curr->left && !curr->right) {
            buffer[byte++] = curr->symbol;
            file_size -= 1;
            curr = root;
            if (byte == BLOCK) {
                write_bytes(outfile, buffer, BLOCK);
                byte = 0;
            }
        }
    }

    if (byte > 0) {
        write_bytes(outfile, buffer, byte);
    }
}

int main(int argc, char **argv) {
    int opt;
    bool verbose = false, visual = false;
    int infile = STDIN_FILENO, outfile = STDOUT_FILENO;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 't':
            visual = true;
            break;
        case 'v':
            verbose = true;
            break;
        case 'i':
            if ((infile = open(optarg, O_RDONLY)) == -1) {
                return 1;
            }
            break;
        case 'o':
            if ((outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) {
                return 1;
            }
            break;
        default:
            return 1;
        }
    }

    Header h;
    read_header(infile, &h);
    if (h.magic != MAGIC) {
        fprintf(stderr, "Invalid magic number.\n");
        return 1;
    }
    fchmod(outfile, h.permissions);

    static uint8_t tree[MAX_TREE_SIZE];
    read_bytes(infile, tree, h.tree_size);
    Node *root = huffman_rebuild_tree(h.tree_size, tree);

    write_symbols(infile, outfile, root, h.file_size);

    if (visual) {
        huffman_print_tree(root);
    }

    if (verbose) {
        double ratio = 1.0 - (double)bytes_read / (double)bytes_written;
        fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", bytes_read);
        fprintf(stderr, "Uncompressed file size: %" PRIu64 " bytes\n", bytes_written);
        fprintf(stderr, "Space saving: %.2lf%%\n", 100.0 * ratio);
    }

    huffman_delete_tree(&root);
    close(infile);
    close(outfile);
    return 0;
}
