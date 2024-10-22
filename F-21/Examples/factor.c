#include <stdio.h>

void factor(int n, int k) {
    if (k > n) {
        return;
    } else if (n % k == 0) {
        printf("%d ", k);
        factor(n / k, k);
    } else {
        factor(n, k + 1);
    }
}
int main(void) {
    int n;
    printf("?? ");
    scanf("%d", &n);
    factor(n, 2);
    puts("\n");
}


