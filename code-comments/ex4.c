// BSD License
#include "LICENSE.h"

#include <stdio.h>

int main(void) {
    char *s = "Quis custodiet ipsos custodes?";

    printf("%s\n", s);
    for (int i = 0; i < 10; i += 1) {
        printf("%c", s[i]);
    }
    printf("\n");
    for (int i = 0; i < 10; i += 1) {
        printf("%c", *(s + i));
    }
    printf("\n");
    return 0;
}
