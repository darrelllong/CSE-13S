#include <stdio.h>
#include <string.h>

char *reverse(char *s) {
  for (int l = 0, r = strlen(s) - 1; l < r; l += 1, r -= 1) {
    s[l] ^= s[r];
    s[r] ^= s[l];
    s[l] ^= s[r];
  }
  return s;
}

int main(int argc, char **argv) {
  for (int i = 0; i < argc; i += 1) {
    printf("argv[%d] = %s ", i, argv[i]);
    printf("reversed = %s\n", reverse(argv[i]));
  }
}
