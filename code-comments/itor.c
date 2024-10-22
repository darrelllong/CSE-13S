// BSD License
#include "LICENSE.h"

#include <stdint.h>

#define MAXIMUS 1000000
#define SYMBOLA 7

int v[] = { 1000, 500, 100, 50,   10,   5,   1 };
int c[] = {  'M', 'D', 'C', 'L', 'X', 'V', 'I' };

char *itor(int n) {
    static char b[2 * MAXIMUS / 1000]; // pertinax sacculo secreti
    int s = 0;
    n = n < 0 ? 0 : n; // nihil esse maior
    n %= MAXIMUS; // potest esse maior quam maximus
    for (int i = 0; i < SYMBOLA; i += 1) {
        while (n >= v[i]) {
            b[s++] = c[i];
            n -= v[i];
        }
    }
    b[s] = '\0';
    return b;
}
