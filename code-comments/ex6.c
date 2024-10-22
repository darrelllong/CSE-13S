// BSD License
#include "LICENSE.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

bool isPrime(n) // Incredibly naive
{
    for (int i = 2; i < sqrtl(n); i += 1) {
        if (n % i == 0) {
            return false;
        }
    }
    return true;
}

int main(void) {
    int x;
    printf("Enter x: ");
    scanf("%d", &x);
    printf("%d is ", x);
    if (isPrime(x)) {
        printf("prime!\n");
    } else {
        printf("composite!\n");
    }
    return 0;
}
