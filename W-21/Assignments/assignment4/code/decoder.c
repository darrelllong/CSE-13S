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

static uint32_t total_bytes, errs, ok_errs;

uint8_t decode(uint8_t encoded) {
    uint8_t rc;
    uint8_t val = 0;

    total_bytes++;

    rc = ham_decode(encoded, &val);
    switch (rc) {
    case HAM_ERR: errs++; break;
    case HAM_ERR_OK: ok_errs++; break;
    default: break;
    }

    return val;
}

int main(int argc, char **argv) {
    uint8_t in_buf[kB * 2];
    uint8_t out_buf[kB];
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

    int rd = read(in_fd, in_buf, kB * 2);
    while (rd) {
        for (int i = 0; i < rd; i += 2) {
            out_buf[i / 2] = decode(in_buf[i]) & 0xF;
            out_buf[i / 2] |= decode(in_buf[i + 1]) << 4;
        }

        write(out_fd, out_buf, rd / 2);
        rd = read(in_fd, in_buf, kB * 2);
    }

    fprintf(stderr, "Total bytes processed: %u\n", total_bytes);
    fprintf(stderr, "Uncorrected errors: %u\n", errs);
    fprintf(stderr, "Corrected errors: %u\n", ok_errs);
    fprintf(stderr, "Error rate: %f\n", (double) errs / total_bytes);

    close(in_fd);
    close(out_fd);

    return 0;
}
