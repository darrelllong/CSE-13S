#include "defines.h"
#include "huffman.h"
#include "io.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define OPTIONS "vti:o:"

bool seekable(int fd) {
    return !isatty(fd) && lseek(fd, 0, SEEK_CUR) != -1;
}

int copyfile(int src) {
    char fname[BLOCK];

    // Get current identifer as temp file name identifier.
    time_t now = time(NULL);
    struct tm *info = localtime(&now);

    // Fake a mkstemp() since it's not ANSI.
    snprintf(fname, BLOCK, "/tmp/huffman.%s", asctime(info));
    int dst = open(fname, O_RDWR | O_CREAT | O_TRUNC | O_EXCL, 0600);
    unlink(fname);

    int bytes = 0;
    uint8_t buf[BLOCK];
    while ((bytes = read_bytes(src, buf, BLOCK)) > 0) {
        write_bytes(dst, buf, bytes);
    }

    // Reset the copied file for use and global statistics counter.
    lseek(dst, 0, SEEK_SET);
    bytes_written = 0;
    close(src);
    return dst;
}

int load_hist(int infile, uint64_t hist[static ALPHABET]) {
    int bytes = 0;
    int unique = 0;
    uint8_t buffer[BLOCK];

    do {
        bytes = read_bytes(infile, buffer, BLOCK);
        for (int i = 0; i < bytes; i += 1) {
            if (hist[buffer[i]] == 0) {
                unique += 1;
            }
            hist[buffer[i]] += 1;
        }
    } while (bytes > 0);

    // Ensure at least two nodes.
    if (unique == 0) {
        hist[0x00] = hist[0xFF] = 1;
    } else if (unique == 1) {
        hist[0x00] > 0 ? (hist[0xFF] = 1) : (hist[0x00] = 1);
    }

    // Seek back to the start and return number of unique symbols.
    lseek(infile, 0, SEEK_SET);
    return unique;
}

void write_tree(int outfile, Node *root, uint16_t tree_size) {
    static int tree_ind = 0;
    static uint8_t tree[MAX_TREE_SIZE];

    if (root) {
        // Dumping the tree requires a postorder traversal.
        write_tree(outfile, root->left, tree_size);
        write_tree(outfile, root->right, tree_size);

        // Leaves are represented with a 'L' followed by the symbol.
        // Internal nodes are represented with an 'I'.
        if (!root->left && !root->right) {
            tree[tree_ind++] = 'L';
            tree[tree_ind++] = root->symbol;
        } else {
            tree[tree_ind++] = 'I';
        }

        // This will only happen once after the last internal node.
        if (tree_ind == tree_size) {
            write_bytes(outfile, tree, tree_size);
        }
    }
}

void write_codes(int infile, int outfile, Code table[static ALPHABET]) {
    int bytes = 0;
    bytes_read = 0; // Reset global counter for second pass.
    uint8_t buffer[BLOCK];

    do {
        bytes = read_bytes(infile, buffer, BLOCK);
        for (int i = 0; i < bytes; i += 1) {
            write_code(outfile, &table[buffer[i]]);
        }
    } while (bytes > 0);

    flush_codes(outfile);
}

int main(int argc, char **argv) {
    int opt;
    bool verbose = false, visual = false;
    int infile = STDIN_FILENO, outfile = STDOUT_FILENO;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v':
            verbose = true;
            break;
        case 't':
            visual = true;
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

    if (!seekable(infile)) {
        infile = copyfile(infile);
    }

    // Need to grab permissions and input file size.
    struct stat statbuf;
    fstat(infile, &statbuf);
    fchmod(outfile, statbuf.st_mode);

    Code table[ALPHABET] = { 0 };
    uint64_t hist[ALPHABET] = { 0 };

    // Create histogram, build the tree, build the codes.
    int unique = load_hist(infile, hist);
    Node *root = huffman_build_tree(hist);
    huffman_build_code(root, table);

    Header h = {
        .magic = MAGIC,
        .permissions = statbuf.st_mode,
        .tree_size = 3 * unique - 1,
        .file_size = statbuf.st_size,
    };

    // Write the header, write the tree, walk the input and write codes.
    write_header(outfile, &h);
    write_tree(outfile, root, h.tree_size);
    write_codes(infile, outfile, table);

    if (visual) {
        huffman_print_tree(root);
    }

    if (verbose) {
        double ratio = 1.0 - (double)bytes_written / (double)bytes_read;
        fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", bytes_written);
        fprintf(stderr, "Uncompressed file size: %" PRIu64 " bytes\n", bytes_read);
        fprintf(stderr, "Space saving: %.2lf%%\n", 100.0 * ratio);
    }

    huffman_delete_tree(&root);
    close(infile);
    close(outfile);
    return 0;
}
