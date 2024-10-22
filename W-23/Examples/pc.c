#include <stdio.h>
#include <strings.h>

#define MAX 100

void c(char *b) {
  int l = strlen(b);
  for (int i = 1; i < 1 << l; i += 1) {
    for (int j = 0; j < l; j += 1) {
      if (i & 1 << j) {
        printf("%c", b[j]);
      }
    }
    printf("\n");
  }
  return;
}

void swap(char *a, char *b) {
  char t = *a; *a = *b; *b = t;
  return;
}

void p(char *b, int n) {
  int l = strlen(b);
  if (n == l) {
    puts(b);
  } else {
    for (int i = n; i < l; i += 1) {
      swap(&b[n], &b[i]);
      p(b, n + 1);
      swap(&b[n], &b[i]);
    }
  }
  return;
}

int main(void) {
  char b[MAX];
  printf("string: ");
  fgets(b, MAX, stdin);
  b[strlen(b) - 1] = '\0'; // remove \n
  printf("Combinations\n"
         "------------\n");
  c(b);
  printf("Permutations\n"
         "------------\n");
  p(b, 0);
  return 0;
}
