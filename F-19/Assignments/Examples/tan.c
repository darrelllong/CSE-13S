#include <math.h>
#include <stdio.h>

#define PI 3.141592653589793238462643383279502884197

double Tan(double x) {
  double n = x * (-654729075 + x * x * (91891800 + x * x * (-2837835 + 
             x * x * (25740 - 55 * x * x))));
  double d = -654729075 + x * x * (310134825 + x * x * (-18918900 +
             x * x * (315315 + x * x * (-1485 + x * x))));
  return n / d;
}


#define DELTA 0.001

int main(void) {
  printf("x        Tan             Library         Difference\n");
  printf("-        ---             -------         ----------\n");
  for (double r = -0.5 * PI + DELTA; r <= 0.5 * PI - DELTA; r += PI / 16) {
    printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", r, Tan(r), tan(r),
           Tan(r) - tan(r));
  }
}

