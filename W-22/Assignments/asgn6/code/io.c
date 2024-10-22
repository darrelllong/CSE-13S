#include "defines.h"
#include "io.h"
#include <unistd.h>
#include <string.h>

#define SETBIT(A, x) (A[(x) / 8] |= (1 << ((x) % 8)))
#define CLRBIT(A, x) (A[(x) / 8] &= ~(1 << ((x) % 8)))
#define GETBIT(A, x) ((A[(x) / 8] >> ((x) % 8)) & 1)

static int bitind = 0;
static uint8_t bitbuf[BLOCK];

// For statistics.
uint64_t bytes_read    = 0;
uint64_t bytes_written = 0;

int read_bytes(int fd, uint8_t *buf, int nbytes) {
    int bytes = 0, total = 0;

    do {
        bytes = read(fd, buf + total, nbytes - total);
        total += bytes;
    } while (bytes > 0 && total != nbytes);

    bytes_read += total;
    return total;
}

int write_bytes(int fd, uint8_t *buf, int nbytes) {
    int bytes = 0, total = 0;

    do {
        bytes = write(fd, buf + total, nbytes - total);
        total += bytes;
    } while (bytes > 0 && total != nbytes);

    bytes_written += total;
    return total;
}

bool read_bit(int infile, uint8_t *bit) {
    static int EOB = -1;

    if (bitind == 0) {
        int bytes = read_bytes(infile, bitbuf, BLOCK);
        if (bytes != BLOCK) {
            EOB = 8 * bytes + 1;
        }
    }

    *bit   = GETBIT(bitbuf, bitind);
    bitind = (bitind + 1) % (8 * BLOCK);

    return bitind != EOB;
}

void write_code(int outfile, Code *c) {
    for (uint32_t i = 0; i < code_size(c); i += 1) {
        if (GETBIT(c->bits, i)) {
            SETBIT(bitbuf, bitind);
        }
        bitind += 1;
        if (bitind == 8 * BLOCK) {
            write_bytes(outfile, bitbuf, BLOCK);
            memset(bitbuf, 0, BLOCK);
            bitind = 0;
        }
    }
}

void flush_codes(int outfile) {
    if (bitind > 0) {
        int bytes = bitind % 8 == 0 ? bitind / 8 : bitind / 8 + 1;
        write_bytes(outfile, bitbuf, bytes);
    }
}
