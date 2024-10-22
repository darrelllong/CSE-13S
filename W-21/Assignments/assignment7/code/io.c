#include "code.h"
#include "endian.h"
#include "io.h"
#include "util.h"
#include "word.h"
#include <unistd.h>
#include <string.h>

#define BYTE 8 // Byte is 8 bits.
#define SHORT 16 // Short is 16 bits.

uint64_t total_syms = 0;
uint64_t total_bits = 0;

static int syms = 0;
static uint8_t symbuf[BLOCK] = { 0 };

static int bits = 0;
static uint8_t bitbuf[BLOCK] = { 0 };

int read_bytes(int infile, uint8_t *buf, int to_read) {
    int bytes_to_read = to_read;
    int bytes_read = 0;
    int total_read = 0;

    do {
        bytes_read = read(infile, buf + total_read, bytes_to_read);
        check(bytes_read != -1, "Failed to read from input file.\n");
        bytes_to_read -= bytes_read;
        total_read += bytes_read;
    } while (bytes_read && total_read != to_read);

    return total_read;
}

int write_bytes(int outfile, uint8_t *buf, int to_write) {
    int bytes_to_write = to_write;
    int bytes_written = 0;
    int total_written = 0;

    do {
        bytes_written = write(outfile, buf + total_written, bytes_to_write);
        check(bytes_written != -1, "Failed to write to output file.\n");
        bytes_to_write -= bytes_written;
        total_written += bytes_written;
    } while (bytes_written && total_written != to_write);

    return total_written;
}

void read_header(int fd, FileHeader *fh) {
    read_bytes(fd, (uint8_t *)fh, sizeof(FileHeader));
    if (big_endian()) {
        fh->magic = swap32(fh->magic);
        fh->protection = swap16(fh->protection);
    }
    total_bits += (BYTE * sizeof(FileHeader));
}

void write_header(int fd, FileHeader *fh) {
    total_bits += (BYTE * sizeof(FileHeader));
    if (big_endian()) {
        fh->magic = swap32(fh->magic);
        fh->protection = swap16(fh->protection);
    }
    write_bytes(fd, (uint8_t *)fh, sizeof(FileHeader));
}

bool read_sym(int fd, uint8_t *s) {
    static int end = -1;

    if (!syms) {
        int bytes = read_bytes(fd, symbuf, BLOCK);

        if (bytes != BLOCK) {
            end = bytes + 1;
        }
    }

    *s = symbuf[syms];
    syms = (syms + 1) % BLOCK;

    if (syms != end) {
        total_syms += 1;
    }

    return syms != end;
}

void write_pair(int fd, uint16_t c, uint8_t s, int width) {
    total_bits += BYTE + width;

    if (big_endian()) {
        c = swap16(c);
    }

    for (int i = 0; i < width; i += 1) {
        if (c & (1 << i)) {
            bitbuf[bits / BYTE] |= (1 << (bits % BYTE));
        }

        bits += 1;

        if (bits / BYTE == BLOCK) {
            write_bytes(fd, bitbuf, BLOCK);
            memset(bitbuf, 0, BLOCK);
            bits = 0;
        }
    }

    for (int i = 0; i < BYTE; i += 1) {
        if (s & (1 << i)) {
            bitbuf[bits / BYTE] |= (1 << (bits % BYTE));
        }

        bits += 1;

        if (bits / BYTE == BLOCK) {
            write_bytes(fd, bitbuf, BLOCK);
            memset(bitbuf, 0, BLOCK);
            bits = 0;
        }
    }
}

void flush_pairs(int fd) {
    if (bits) {
        write_bytes(fd, bitbuf, bytes(bits));
    }
}

bool read_pair(int fd, uint16_t *c, uint8_t *s, int width) {
    total_bits += BYTE + width;

    *c = 0;
    *s = 0;

    for (int i = 0; i < width; i += 1) {
        if (!bits) {
            read_bytes(fd, bitbuf, BLOCK);
        }

        if (bitbuf[bits / BYTE] & (1 << (bits % BYTE))) {
            *c |= (1 << i);
        }

        bits = (bits + 1) % (BLOCK * BYTE);
    }

    for (int i = 0; i < BYTE; i += 1) {
        if (!bits) {
            read_bytes(fd, bitbuf, BLOCK);
        }

        if (bitbuf[bits / BYTE] & (1 << (bits % BYTE))) {
            *s |= (1 << i);
        }

        bits = (bits + 1) % (BLOCK * BYTE);
    }

    if (big_endian()) {
        *c = swap16(*c);
    }

    return *c != STOP_CODE;
}

void write_word(int fd, Word *w) {
    total_syms += w->len;

    for (uint32_t i = 0; i < w->len; i += 1) {
        symbuf[syms++] = w->syms[i];

        if (syms == BLOCK) {
            write_bytes(fd, symbuf, BLOCK);
            syms = 0;
        }
    }
}

void flush_words(int fd) {
    if (syms) {
        write_bytes(fd, symbuf, syms);
    }
}
