#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    uint64_t data[2]; // low = data[0], high = data[1]
} uint128_t;

typedef struct {
    uint128_t a, b, c;
} state;

static inline uint64_t getBit(uint128_t *n, uint64_t k) {
    return (n->data[k / 64] >> (k % 64)) & 0x1;
}

static inline void setBit(uint128_t *n, uint64_t k) {
    n->data[k / 64] |= ((uint64_t) 0x1 << k % 64);
    return;
}

static inline void clrBit(uint128_t *n, uint64_t k) {
    n->data[k / 64] &= ~((uint64_t) 0x1 << k % 64);
    return;
}

static inline void asgnBit(uint128_t *n, uint64_t k, uint64_t v) {
    if (v) {
        setBit(n, k);
    } else {
        clrBit(n, k);
    }
    return;
}

static inline void load80Bits(uint128_t *n, uint64_t low, uint64_t high) {
    n->data[0] = low;
    n->data[1] = high & 0xffff000000000000; // Only allow 80 bits per the specification
    return;
}

static inline void leftShift(uint128_t *n) {
    n->data[1] = n->data[1] << 1 | ((n->data[0] >> 63) & 0x1); // high bit of low → low of high
    n->data[0] = n->data[0] << 1;                              // left shift low
    return;
}

void startTrivium(state *s, uint128_t key, uint128_t nonce) {
    memset(s, 0, sizeof(state));
    setBit(&s->c, 108);
    setBit(&s->c, 109);
    setBit(&s->c, 110);
    load80Bits(&s->a, key.data[0], key.data[1]);
    load80Bits(&s->b, nonce.data[0], nonce.data[1]);
    return;
}

uint64_t Trivium(state *s) {
    uint64_t inA, outA;
    uint64_t inB, outB;
    uint64_t inC, outC;
    outA = getBit(&s->a, 65) ^ getBit(&s->a,  92) ^ (getBit(&s->a,  90) & getBit(&s->a,  91));
    outB = getBit(&s->b, 68) ^ getBit(&s->b,  83) ^ (getBit(&s->b,  81) & getBit(&s->b,  82));
    outC = getBit(&s->c, 65) ^ getBit(&s->c, 110) ^ (getBit(&s->c, 108) & getBit(&s->c, 109));
    inA = getBit(&s->a, 68) ^ outC;
    inB = getBit(&s->b, 77) ^ outA;
    inC = getBit(&s->c, 86) ^ outB;
    leftShift(&s->a);
    leftShift(&s->b);
    leftShift(&s->c);
    asgnBit(&s->a, 0, inA);
    asgnBit(&s->b, 0, inB);
    asgnBit(&s->c, 0, inC);
    return outA ^ outB ^ outC;
}

void warmTrivium(state *s) {
    for (int i = 0; i < 4 * 288; i += 1) { // Cycle it four times per the specification
        (void) Trivium(s);
    }
    return;
}

int main(void) {
    state s;
    uint128_t key = { 0xcc42211804b3d194, 0xd642738d7e95158d };
    uint128_t nonce = { 0xe8e420f3b4ae270e, 0x8f0fb334746e1c0d };
    nonce.data[0] = time(NULL);
    startTrivium(&s, key, nonce);
    warmTrivium(&s);
    for (int i = 0; i < 256; i += 1) {
        printf("%lld", Trivium(&s));
    }
    printf("\n");
    uint64_t sum = 0;
#define BIG 1e7
    for (int i = 0; i < BIG; i += 1) {
        sum += Trivium(&s);
    }
    printf("%lf\n", sum / BIG);
    return 0;
}
