// BSD License
#include "LICENSE.h"

#include <stdio.h>

int main(void) {
    int x, *p;
    x = 1234;
    p = &x;
    printf("x = %d, &x = %x, p = %x\n", x, (unsigned) &x, (unsigned) p);
    *p = 9876;
    printf("x = %d, &x = %x, p = %x\n", x, (unsigned) &x, (unsigned) p);
    return 0;
}
