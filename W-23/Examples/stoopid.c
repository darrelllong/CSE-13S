#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX 10

static long long unsigned int ops = 0;

void fill(long *a, int n) {
  srandom(time((time_t *)0));
  for (int i = 0; i < n; i += 1) {
    a[i] = random() % 30;
  }
  return;
}

bool is_sorted(long *a, int n) {
  for (int i = 1; i < n; i += 1) {
    if (a[i - 1] > a[i]) {
      return false;
    }
  }
  return true;
}

static inline void swap(long *a, long *b) {
  ops += 1;
  if (a != b) { *a ^= *b; *b ^= *a; *a ^= *b; }
  return;
}

void sort(long *a, int n) {
  while (!is_sorted(a, n)) {
    swap(a + random() % MAX, a + random() % MAX);
  }
}

void print(long *a, int n) {
  for (int i = 0; i < n; i += 1) {
    printf("a[%d] = %ld\n", i, a[i]);
  }
  return;
}

int main(void) {
  long a[MAX];
  fill(a, MAX);
  puts("Original");
  puts("--------");
  print(a, MAX);
  sort(a, MAX);
  fprintf(stderr, "\n%lld\n\n", ops);
  puts("Sorted");
  puts("------");
  print(a, MAX);
  return 0;
}
