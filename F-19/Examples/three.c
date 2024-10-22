#include <stdbool.h>
#include <stdio.h>

int a[] = {-10, -8, -6, -4, -2, 0, 1, 3, 5, 7, 9};

int main(void) {
    int big = a[0], little = a[0];
    for (int i = 1; i <= 10; i += 1) {
        if (big < a[i]) {
            big = a[i];
        }
        if (little > a[i]) {
            little = a[i];
        }
    }
    printf("Biggest = %d, littlest = %d\n", big, little);
    return 0;
}
