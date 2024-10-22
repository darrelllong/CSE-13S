#include <stdio.h>
#include <stdlib.h>

#define SEED 2021

int main(void) {
    for (int i = 0; i < 3; i += 1) {
        puts("Set the random seed.");
        srandom(SEED);
        for (int i = 0; i < 5; i += 1) {
            printf(" - generated %lu\n", random());
        }
    }
}
