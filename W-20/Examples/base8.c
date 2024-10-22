#include <inttypes.h>
#include <stdio.h>

#define OCTAL_BYTES 24 // Log[18446744073709551616] / Log[8] + 1 + 1

char *octal(uint64_t n) {
  static char s[OCTAL_BYTES];
  char *t = s + OCTAL_BYTES;
  *--t = '\0';
  do {
    *--t = n % 8 + '0';
    n /= 8;
  } while (n > 0);

  return t;
}

int main(void) {

  for (uint32_t i = 0; i < 256; i += 1) {
    printf("%d = %s(8)\n", i, octal(i));
  }

  return 0;
}
