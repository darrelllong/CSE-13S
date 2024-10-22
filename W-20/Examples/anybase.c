#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ANY_BYTES 66 // 64 bits + 1 + 1

static char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

char *anyBase(uint64_t n, uint32_t b) {
  static char s[ANY_BYTES];
  char *t = s + ANY_BYTES;
  b = b < 2 ? 2 : b; // Valid base
  *--t = '\0';
  do {
    *--t = digits[n % b];
    n /= b;
  } while (n > 0);

  return t;
}

#define OPTIONS "n:"

int main(int argc, char **argv) {
  uint32_t n = 10;

  int c;
  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 'n':
      n = strtol(optarg, (char **)NULL, 10);
      break;
    default:
      return -1;
    }
  }

  for (uint32_t i = 0; i < 256; i += 1) {
    printf("%d = %s(%d)\n", i, anyBase(i, n), n);
  }

  return 0;
}
