#include "defines.h"
#include "header.h"
#include "huffman.h"
#include "io.h"
#include <fcntl.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define OPTIONS "vhi:o:"

bool seekable(int fd) {
    return !isatty(fd) && lseek(fd, 0, SEEK_CUR) != -1;
}

int copyfile(int src) {
    char fname[BLOCK];

    // Get current identifer as temp file name identifier.
    time_t now      = time(NULL);
    struct tm *info = localtime(&now);

    // Fake a mkstemp() since it's not ANSI.
    snprintf(fname, BLOCK, "/tmp/huffman.%s", asctime(info));
    int dst = open(fname, O_RDWR | O_CREAT | O_TRUNC | O_EXCL, 0600);
    unlink(fname);

    int bytes = 0;
    uint8_t buf[BLOCK];
    for (;;) {
        bytes = read_bytes(src, buf, BLOCK);
        write_bytes(dst, buf, bytes);
        if (bytes < BLOCK) {
            break;
        }
    }

    // Reset the copied file for use and global statistics counter.
    lseek(dst, 0, SEEK_SET);
    bytes_written = 0;
    close(src);
    return dst;
}

int load_hist(int infile, uint64_t hist[static ALPHABET]) {
    int bytes  = 0;
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
    // Could be better, but just simpler for students to follow.
    if (hist[0x00] == 0) {
        unique += 1;
    }
    if (hist[0xFF] == 0) {
        unique += 1;
    }
    hist[0x00] += 1;
    hist[0xFF] += 1;

    // Seek back to the start and return number of unique symbols.
    lseek(infile, 0, SEEK_SET);
    return unique;
}

void write_codes(int infile, int outfile, Code table[static ALPHABET]) {
    int bytes  = 0;
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

void usage(char *exec) {
    fprintf(stderr,
            "SYNOPSIS\n"
            "  A Huffman encoder.\n"
            "  Compresses a file using the Huffman coding algorithm.\n"
            "\n"
            "USAGE\n"
            "  %s [-h] [-i infile] [-o outfile]\n"
            "\n"
            "OPTIONS\n"
            "  -h             Program usage and help.\n"
            "  -v             Print compression statistics.\n"
            "  -i infile      Input file to compress.\n"
            "  -o outfile     Output of compressed data.\n",
            exec);
}

int main(int argc, char **argv) {
    int opt;
    bool verbose = false;
    int infile = STDIN_FILENO, outfile = STDOUT_FILENO;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v':
            verbose = true;
            break;
        case 'i':
            if ((infile = open(optarg, O_RDONLY)) == -1) {
                return EXIT_FAILURE;
            }
            break;
        case 'o':
            if ((outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1) {
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

    if (!seekable(infile)) {
        infile = copyfile(infile);
    }

    // Need to grab permissions and input file size.
    struct stat statbuf;
    fstat(infile, &statbuf);
    fchmod(outfile, statbuf.st_mode);

    Code table[ALPHABET]    = { 0 };
    uint64_t hist[ALPHABET] = { 0 };

    // Create histogram, build the tree, build the codes.
    int unique = load_hist(infile, hist);
    Node *root = build_tree(hist);
    build_codes(root, table);

    Header h = {
        .magic       = MAGIC,
        .permissions = statbuf.st_mode,
        .tree_size   = 3 * unique - 1,
        .file_size   = statbuf.st_size,
    };

    // Write the header, write the tree, walk the input and write codes.
    write_bytes(outfile, (uint8_t *)&h, sizeof(Header));
    dump_tree(outfile, root);
    write_codes(infile, outfile, table);

    if (verbose) {
        double ratio = 1.0 - (double)bytes_written / (double)bytes_read;
        fprintf(stderr, "Uncompressed file size: %" PRIu64 " bytes\n", bytes_read);
        fprintf(stderr, "Compressed file size: %" PRIu64 " bytes\n", bytes_written);
        fprintf(stderr, "Space saving: %.2lf%%\n", 100.0 * ratio);
    }

    delete_tree(&root);
    close(infile);
    close(outfile);
    return EXIT_SUCCESS;
}
