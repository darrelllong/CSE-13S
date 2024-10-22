#include "bv64.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double hickoryDickory(void)
{
    struct timeval    t;
    gettimeofday(&t, (struct timezone *) 0);
    return t.tv_sec + t.tv_usec / 1000000.0;
}

#define BIG (1<<30)

int main(void) {
    bitV *b = newVec(BIG);
    for (int i = 1; i < 100; i += 1) {
        setBit(b, BIG / i - 1 );
        printf("parity(%d bits) = %" PRIu64 "\n", i, parity(b));
    }
    bitV *c = newVec(BIG);
    printf("Generating..."); fflush(stdout);
    double start = hickoryDickory();
    for (int i = 0; i < BIG; i += 1) { setBit(c, random() % BIG); }
    double stop = hickoryDickory();
    printf("done (%lfs)\n", (stop - start)); fflush(stdout);
    printf("%d random bits, parity = %" PRIu64 "\n", BIG, parity(c));
    start = hickoryDickory();
    for (int i = 0; i < 1000; i += 1) {
        setBit(c, i);
        (void) parity(c);
    }
    stop = hickoryDickory();
    printf("%u parity calculation in %lfs\n", BIG, (stop - start) / 1000);
    return 0;
}
