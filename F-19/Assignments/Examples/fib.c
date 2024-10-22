#include <stdio.h>

int fib_1(int k) {
    if (k == 0 || k == 1) {
        return k;
    } else {
        return fib_1(k - 1) + fib_1(k - 2);
    }
}

int fib_2(int k) {
    int a = 0, b = 1, s = 0;
    if (k < 2) {
        return k;
    }
    for (int i = 2; i <= k; i += 1) {
        s = a + b;
        a = b;
        b = s;
    }
    return s;
}

int main(void) {
    for (int i = 0; i < 20; i += 1) {
        printf("fib(%d) = %d = %d\n", i, fib_1(i), fib_2(i));
    }
    return 0;
}
