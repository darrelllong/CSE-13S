#include <stdio.h>

int sum_1(int k) {
  if (k > 1) {
    return k + sum_1(k - 1);
  } else {
    return 1;
  }
}

int sum_2(int k) {
  int s = 1;
  for (int i = 2; i <= k; i += 1) {
    s += i;
  }
  return s;
}

int sum_3(int k) {
  return (k * (k + 1)) / 2;
}

int main(void) {
printf("sum(10) = %d = %d = %d\n", sum_1(10), sum_2(10), sum_3(10));
return 0;
}
