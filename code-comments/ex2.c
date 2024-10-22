// BSD License
#include "LICENSE.h"

#include <stdio.h>

int main(void) {
    int n, sum;
    printf("Enter in: ");
    scanf("%d", &n);
    printf("1");
    sum = 1;
    for (int i = 2; i <= n; i += 1) {
        printf(" + %d", i);
        sum = sum + i;
    }
    printf(" = %d\n", sum);
    printf("Gauss said %d\n", (n * (n + 1)) / 2);
    return 0;
}
