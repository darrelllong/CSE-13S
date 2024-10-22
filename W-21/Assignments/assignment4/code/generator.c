#include "bv.h"
#include "hamming.h"

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define STDIN   0
#define STDOUT  1
#define OPTIONS "i:o:h"
#define kB      1024
#define NUM_HC  16

int main(int argc, char **argv) {
    uint8_t ham_lt[NUM_HC] = { 0 };
    uint8_t out_buf[kB * 2];
    uint8_t in_buf[kB];
    int out_fd = STDOUT;
    int in_fd = STDIN;
    int c;

    while ((c = getopt(argc, argv, OPTIONS)) != -1) {
        switch (c) {
        case 'i':
            in_fd = open(optarg, O_RDONLY);
            if (in_fd < 0) {
                perror(optarg);
                exit(1);
            }
            break;
        case 'o':
            out_fd = open(optarg, O_WRONLY | O_CREAT, 0644);
            if (out_fd < 0) {
                perror(optarg);
                exit(1);
            }
            break;
        case 'h':
        default:
            fprintf(stderr, "Usage: %s [-i input file] [-o output file]\n", argv[0]);
            exit(1);
            break;
        }
    }

    if (ham_init()) {
        printf("Initializing Hamming library failed\n");
        exit(1);
    }

    for (uint8_t x = 1; x < NUM_HC; x++) {
        ham_lt[x] = ham_encode(x);
    }

    int rd = read(in_fd, in_buf, kB);
    while (rd) {
        for (int i = 0; i < rd * 2; i += 2) {
            out_buf[i] = ham_lt[in_buf[i / 2] & 0xF];
            out_buf[i + 1] = ham_lt[in_buf[i / 2] >> 4];
        }

        write(out_fd, out_buf, rd * 2);
        rd = read(in_fd, in_buf, kB);
    }

    close(in_fd);
    close(out_fd);

    return 0;
}
