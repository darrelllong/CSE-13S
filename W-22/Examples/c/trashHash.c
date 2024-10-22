#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>

#define RL(x, y)  (((x) << (y)) | ((uint64_t)(x) >> (64 - (y))))
#define RR(x, y)  (((x) >> (y)) | ((uint64_t)(x) << (64 - (y))))

#define HFP 4611686018427388039
#define MXA 6364136223846793005
#define MXC 1442695040888963407
#define SX3 (64 - 1)

uint64_t hash(uint8_t b[], uint32_t l) {
    union{
        uint64_t h;
        uint32_t w[2];
        uint16_t s[4];
        uint8_t  b[8];
    } v;
    v.h = 0;
    for (int i = 0; i < l; i += 1) {
        v.w[0] = v.s[1] & ~v.s[3];
        v.w[1] = v.s[0] & ~v.s[2];
        v.h = (v.h - b[i]) * MXA + MXC;
        v.h ^= RL(HFP, ~b[i] & SX3);
    }
    return v.h;
}

#define KB 1024

int main(void) {
    char b[KB];
    while (fgets(b, KB, stdin) > 0) {
        uint64_t h = hash((uint8_t *)b, strlen(b));
        printf("h(%s) = %" PRIu64 " (%" PRIu64 ")\n", b, h, h % 1000);
    }
    return EXIT_SUCCESS;
}
