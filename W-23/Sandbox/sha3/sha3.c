#include "sha3.h"
#include <string.h>

#define ROUNDS  24    // Number of KECCAK rounds to perform for SHA3-256.
#define WIDTH   200   // 1600-bit width in bytes.
#define LANES   25    // State is an unrolled 5x5 array of 64-bit lanes.
#define RATE    136   // 1600-bit width - 512-bit capacity in bytes.

#define ROTL64(x, y)  (((x) << (y)) | ((x) >> (64 - (y))))

static size_t padpoint = 0;
static size_t absorbed = 0;

static union {
    uint64_t words[LANES];
    uint8_t bytes[WIDTH];
} state;

static void theta(void) {
    uint64_t C[5] = { 0 };
    uint64_t D[5] = { 0 };

    for (size_t i = 0; i < 5; i += 1) {
        C[i]  = state.words[i];
        C[i] ^= state.words[i + 5];
        C[i] ^= state.words[i + 10];
        C[i] ^= state.words[i + 15];
        C[i] ^= state.words[i + 20];
    }

    for (size_t i = 0; i < 5; i += 1) {
        D[i] = C[(i + 4) % 5] ^ ROTL64(C[(i + 1) % 5], 1);
    }

    for (size_t i = 0; i < 5; i += 1) {
        for (size_t j = 0; j < 25; j += 5) {
            state.words[i + j] ^= D[i];
        }
    }
}

static void rho(void) {
    static const size_t rotations[25] = {
         0,  1, 62, 28, 27,
        36, 44,  6, 55, 20,
         3, 10, 43, 25, 39,
        41, 45, 15, 21,  8,
        18,  2, 61, 56, 14
    };

    for (size_t i = 0; i < 25; i += 1) {
        state.words[i] = ROTL64(state.words[i], rotations[i]);
    }
}

static void pi(void) {
    static const size_t switcheroo[25] = {
         0, 10, 20,  5, 15,
        16,  1, 11, 21,  6,
         7, 17,  2, 12, 22,
        23,  8, 18,  3, 13,
        14, 24,  9, 19,  4
    };

    uint64_t temp[25] = { 0 };

    for (size_t i = 0; i < 25; i += 1) {
        temp[i] = state.words[i];
    }

    for (size_t i = 0; i < 25; i += 1) {
        state.words[switcheroo[i]] = temp[i];
    }
}

static void chi(void) {
    uint64_t temp[5] = { 0 };

    for (size_t j = 0; j < 25; j += 5) {
        for (size_t i = 0; i < 5; i += 1) {
            temp[i] = state.words[i + j];
        }

        for (size_t i = 0; i < 5; i += 1) {
            state.words[i + j] ^= (~temp[(i + 1) % 5]) & temp[(i + 2) % 5];
        }
    }
}

static void iota(uint8_t r) {
    static const uint64_t constants[24] = {
        0x0000000000000001, 0x0000000000008082, 0x800000000000808a,
        0x8000000080008000, 0x000000000000808b, 0x0000000080000001,
        0x8000000080008081, 0x8000000000008009, 0x000000000000008a,
        0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
        0x000000008000808b, 0x800000000000008b, 0x8000000000008089,
        0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
        0x000000000000800a, 0x800000008000000a, 0x8000000080008081,
        0x8000000000008080, 0x0000000080000001, 0x8000000080008008
    };

    state.words[0] ^= constants[r];
}

static void keccak(void) {
    for (size_t i = 0; i < ROUNDS; i += 1) {
        theta(); rho(); pi(); chi(); iota(i);
    }
}

static void absorb(uint8_t data[], size_t n) {
    for (size_t i = 0; i < n; i += 1) {
        state.bytes[absorbed++] ^= data[i];

        if (absorbed == RATE) {
            keccak();
            absorbed = 0;
        }
    }

    padpoint = absorbed;
}

static void squeeze(uint8_t digest[]) {
    state.bytes[padpoint] ^= 0x06;
    state.bytes[RATE - 1] ^= 0x80;

    keccak();

    for (size_t i = 0; i < SHA3_DIGEST; i += 1) {
        digest[i] = state.bytes[i];
    }

    padpoint = absorbed = 0;
    memset(&state, 0, sizeof(state));
}

void sha3_digest(uint8_t digest[], uint8_t data[], size_t n) {
    absorb(data, n);
    squeeze(digest);
}
