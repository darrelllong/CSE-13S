#include <stdio.h>

int main(void) {
    int c;
    while ((c = getchar()) != EOF) {
        if (c >= 'a' && c <= 'z') {
            putchar(c - ('a' - 'A'));
        } else {
            putchar(c);
        }
    }
    return 0;
}
