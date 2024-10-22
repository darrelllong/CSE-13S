#include <stdio.h>

int list[] = { 9, 3, 2, 7, 5, 1, 0, 4, 6, 8 };

int min_1(int *l, int k) {
  if (k == 1) {
    return *l;
  } else {
    int m = *l++;
    int n = min_1(l, k - 1);
    return m < n ? m : n;
  }
}

int min_2(int *l, int k) {
  int m = *l++;
  for (int i = 1; i < k; i += 1) {
    if (*l < m) {
      m = *l++;
    } else {
      l += 1;
    }
  }
  return m;
}

int main(void) {
  printf("min = %d (%d)\n", min_1(list, 10), min_2(list, 10));
  return 0;
}
