// BSD License
#include "LICENSE.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BIG_PRIME 1073741827 // First prime after 1GB
#define ALIGN     512

unsigned char memory[BIG_PRIME + ALIGN];

double hickoryDickory() {
    struct timeval t;
    gettimeofday(&t, (struct timezone *) 0);
    return t.tv_sec + t.tv_usec / 1000000.0;
}

double walkMemory(int k, bool w) {
    double start = hickoryDickory();
    int shift = ALIGN - (long) memory % ALIGN;

    for (int i = 0, j = 0; i < BIG_PRIME; i += 1, j = (j + k) % BIG_PRIME) {
        unsigned char waste;
        if (w)
            memory[j % BIG_PRIME + shift] = i % 256;
        else
            waste = memory[j % BIG_PRIME + shift];
    }
    return hickoryDickory() - start;
}

int main() {
    printf("Stride     Read    Write\n");
    printf("------     ----    -----\n");
    for (int s = 1; s < 1 << 20; s <<= 1)
        printf("%6d  %7.4lf  %7.4lf\n", s, walkMemory(s, false), walkMemory(s, true));
}
