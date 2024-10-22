#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int opt;
  bool transpose = false;
  while ((opt = getopt(argc, argv, "t")) != -1) {
    switch (opt) {
    case 't': { transpose = true; break; }
    default:  { return 1; }
    }
  }

  int n, m;
  scanf("%d %d\n", &n, &m);
  printf("%d %d\n", n, m);

  for (int i = 0; i < n; i += 1) {
    for (int j = 0; j < m; j += 1) {
      if (fgetc(stdin) == '1') {
        if (transpose) {
          printf("%d %d\n", j, i);
        } else {
          printf("%d %d\n", i, j);
        }
      }
    }
    int c;
    if ((c = fgetc(stdin)) != '\n') {
      fprintf(stderr, "Barf: c = %c\n", c);
      return 1;
    }
  }

  return 0;
}
