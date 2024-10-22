#include <stdio.h>
#include <stdbool.h>

void printNumber(int n) {
    int k = 0;
    bool b = n < 0;
    unsigned int u = n;
    printf("(");
    while (u > 0) {
        if (u & 0x1) { printf("(1 << %d) + ", k); }
        k += 1;
        u /= 2;
    }
    printf("%s)\n", "0");
}

int main(void) {
    int n;
    while (scanf("%d", &n) != EOF) {
        printNumber(n);
    }
    return 0;
}
