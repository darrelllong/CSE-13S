#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define OPTIONS "hs:e:"
#define SEED    2021
#define KB      1024
#define ERROR   0.001 // 1 in 1000 bits are flipped.

static void usage(char *exec) {
    fprintf(stderr,
            "SYNOPSIS\n"
            "  A error-injection (noise) program.\n"
            "\n"
            "USAGE\n"
            "  %s [-h] [-s seed] [-e error_rate]\n"
            "\n"
            "OPTIONS\n"
            "  -h               Program usage and help.\n"
            "  -s seed          Specifies the random seed.\n"
            "  -e error_rate    Specifies the error rate.\n",
            exec);
}

static inline double uniform() {
    return (random() & 0x7fffffff) / (double)0x7fffffff; // Uniform [0, 1]
}

int main(int argc, char **argv) {
    int c             = 0;
    uint32_t seed     = SEED;
    double error_rate = ERROR;

    while ((c = getopt(argc, argv, OPTIONS)) != -1) {
        switch (c) {
        case 's':
            seed = strtoul(optarg, NULL, 10);
            break;
        case 'e':
            error_rate = strtod(optarg, &optarg);
            error_rate = error_rate > 1.0 ? 1.0 : error_rate; // 100% is the maximum error rate.
            break;
        case 'h':
            usage(argv[0]);
            return EXIT_SUCCESS;
        default:
            usage(argv[0]);
            return EXIT_FAILURE;
        }
    }

    int bytes;
    srandom(seed);
    char buffer[KB];

    while ((bytes = read(STDIN_FILENO, buffer, KB)) > 0) {
        for (int i = 0; i < bytes; i += 1) {
            uint8_t mask = 0x1;              // Mask for the 0th bit.
            for (int j = 0; j < 8; j += 1) { // Iterate through the bits.
                double roll = uniform();
                if (roll < error_rate) {
                    buffer[i] ^= mask; // XOR flips bits.
                }
                mask <<= 1; // Mask for the next bit.
            }
        }
        write(STDOUT_FILENO, buffer, bytes);
    }

    return EXIT_SUCCESS;
}
