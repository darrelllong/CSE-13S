// BRANDO — it's what graphs crave. It has electrolytes!

#include <getopt.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv) {
  uint32_t n = 10;
  int c;
  while ((c = getopt(argc, argv, "n:")) != -1) {
    switch (c) {
    case 'n':
      n = atoi(optarg);
      break;
    default:
      fprintf(stderr, "Usage: %s -n <number of pairs>\n", argv[0]);
      exit(-1);
    }
  }

  srandom((long)time(0));
  printf("%c%c\n", 'A', (char)(random() % 26 + 'A'));
  for (uint32_t i = 0; i < n; i += 1) {
    printf("%c%c\n", (char)(random() % 26 + 'A'), (char)(random() % 26 + 'A'));
  }
  printf("%c%c\n", (char)(random() % 26 + 'A'), 'Z');

  return 0;
}
