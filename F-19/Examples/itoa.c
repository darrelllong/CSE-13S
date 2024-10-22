#include <stdbool.h>
#include <stdint.h>

#define LARGEST 22 // log(2^64)/log(10) ~ 19 + sign + zero + safety

char *itoa(int n) {
    static char b[LARGEST];
    char *t = b + LARGEST;
    bool negative = false;
    if (n < 0) {
        n = -n;
        negative = true;
    }
    *--t = '\0';
    do {
        *--t = n % 10 + '0';
        n /= 10;
    } while (n > 0);
    if (negative) {
        *--t = '-';
    }
    return t;
}
