#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char *shift(char *s, int k) {
    char *t = strdup(s);
    for (int i = 0; i < strlen(s); i += 1) {
        if (isalpha(s[i])) {
            if (islower(s[i])) {
                t[i] = ((s[i] - 'a' + k) % 26) + 'A'; // ([0, 25] + k) % 26 + 'A'
            } else {
                t[i] = ((s[i] - 'A' + k) % 26) + 'A'; // ([0, 25] + k) % 26 + 'A'
            }
        } else {
            t[i] = s[i];
        }
    }
    return t;
}

#define MAX 1000

int main(void) {
    char s[MAX];
    fgets(s, MAX, stdin);
    char *t = shift(s, 3);
    puts(t);
    free(t);
    return 0;
}
