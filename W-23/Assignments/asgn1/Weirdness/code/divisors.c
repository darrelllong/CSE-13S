#include <stdio.h>

#define MAX 500000

int main(void) {
    int which = 0, max = 0, divisors;
    for (int i = 2; i < MAX; i += 1) {
        divisors = 0;
        for (int j = 1; j < i; j += 1) {
            if (i % j == 0) {
                divisors += 1;
            }
        }
        if (divisors > max) {
            max = divisors;
            which = i;
        }
    }
    printf("%d\t%d\n", which, max);
    return 0;
}
