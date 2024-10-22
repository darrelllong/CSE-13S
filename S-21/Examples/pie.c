#include <math.h>
#include <stdio.h>

int main(void) {
  printf("n = ");
  int n;
  if (scanf("%d", &n) < 1 || n < 1) {
    fprintf(stderr, "Oops!\n");
    return 1;
  }
  long double p = 0.0;
  for (long double x = 1.0; x < n; x += 1) {
    p += 1 / (x * x);
  }
  printf("%22.20Lf\t%22.20Lf\t%22.20Lf\n", sqrtl(6 * p), (long double)M_PI,
         sqrtl(6 * p) - M_PI);

  return 0;
}
