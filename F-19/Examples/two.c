#include <stdbool.h>
#include <stdio.h>

int a[] = {-10, -8, -6, -4, -2, 0, 1, 3, 5, 7, 9};

bool search(int k, int a[], int left, int right) {
    if (left > right) {
        return false;
    }
    int m = (left + right) / 2;
    if (k == a[m]) {
        return true;
    } else if (k < a[m]) {
        return search(k, a, left, m - 1);
    } else {
        return search(k, a, m + 1, right);
    }
}

int main(void) {
    for (int i = -10; i < 10; i += 1) {
        if (search(i, a, 0, 10)) {
            printf("%d is found\n", i);
        }
    }
    return 0;
}
