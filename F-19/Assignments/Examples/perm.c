#include <inttypes.h>
#include <stdio.h>

void printArray(uint32_t n, char a[]) {
    putchar('{'); putchar(' ');
    for (int i = 0; i < n; i += 1) {
        putchar(a[i]); putchar(' ');
    }
    putchar('}'); putchar('\n');
    return;
}

void permute(uint32_t n, char a[], uint32_t i) {
    if (i == n - 1) {
        printArray(n, a);
    } else {
        for (int j = i; j < n; j += 1) {
            char t = a[i]; a[i] = a[j]; a[j] = t; // Swap
            permute(n, a, i + 1);
            t = a[i]; a[i] = a[j]; a[j] = t; // Swap back
        }
    }
    return;
}

int main(void) {
    char v[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    permute(10, v, 0);
    return 0;
}
