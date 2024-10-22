#include <inttypes.h>
#include <stdio.h>

#define HEX_BYTES 18 // Log[18446744073709551616] / Log[16] + 1 + 1

static char hexadecimal[] = "0123456789ABCDEF";

char *hex(uint64_t n) {
  static char s[HEX_BYTES];
  char *t = s + HEX_BYTES;
  *--t = '\0';
  do {
    *--t = hexadecimal[n % 16];
    n /= 16;
  } while (n > 0);

  return t;
}

int main(void) {

  for (uint32_t i = 0; i < 256; i += 1) {
    printf("%d = %s(16)\n", i, hex(i));
  }

  return 0;
}
