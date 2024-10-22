#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>

#define OPTIONS "s:e:"
#define SEED    2020
#define ERROR   0.001 // 1 in 1000 bits are flipped
#define kB      1024

static inline double uniform() {
    return (random() & 0x7fffffff) / (double) 0x7fffffff; // uniform [0, 1]
}

int main(int argc, char **argv) {
    int c = 0;
    uint32_t seed = SEED;
    double errorRate = ERROR;

    while ((c = getopt(argc, argv, OPTIONS)) != -1) {
        switch (c) {
        case 's': {
            seed = atoi(optarg);
            break;
        }
        case 'e': {
            errorRate = atof(optarg);
            errorRate = errorRate > 1.0 ? 1.0 : errorRate; // 100% is the maximum error rate
            break;
        }
        default: {
            fprintf(stderr, "Usage: %s [-s seed] [-e error rate]\n", argv[0]);
            break;
        }
        }
    }

    srandom(seed);

    char buffer[kB];
    int length;

    while ((length = read(0, buffer, kB)) > 0) {
        for (int i = 0; i < length; i += 1) {
            uint8_t mask = 0x1; // bit 0
            for (int j = 0; j < 8; j += 1) { // each bit in the byte
                double roll = uniform();
                if (roll < errorRate) {
                    buffer[i] ^= mask; // XOR flips bits
                }
                mask <<= 1; // next bit
            }
        }
        write(1, buffer, length);
    }

    return 0;
}
