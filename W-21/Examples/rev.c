#include <stdio.h>
#include <string.h>

char *reverse(char *s) {
  for (char *l = s, *r = s + strlen(s) - 1; l < r; l++, r--) {
    *l ^= *r;
    *r ^= *l;
    *l ^= *r;
  }
  return s;
}

int main(int argc, char **argv) {
  for (int i = 0; i < argc; i += 1) {
    printf("argv[%d] = %s ", i, argv[i]);
    printf("reversed = %s\n", reverse(argv[i]));
  }
}
