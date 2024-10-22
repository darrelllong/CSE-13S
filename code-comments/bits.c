// BSD License
#include "LICENSE.h"

#include <stdio.h>

const char *bits[16] = {  [0] = "0000",  [1] = "0001",  [2] = "0010",  [3] = "0011",
                          [4] = "0100",  [5] = "0101",  [6] = "0110",  [7] = "0111",
                          [8] = "1000",  [9] = "1001", [10] = "1010", [11] = "1011",
                         [12] = "1100", [13] = "1101", [14] = "1110", [15] = "1111",
};

int main(void) {
    printf("10\t  8\t16\t   2\n");
    printf("--\t  -\t--\t   -\n");
    for (int i = 0; i < 16; i += 1) {
        printf("%2d\t%3o\t%2X\t%s\n", i, i, i, bits[i]);
    }
    return 0;
}
