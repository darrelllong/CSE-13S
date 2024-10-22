#include <math.h>
#include <stdio.h>

#define PI 3.141592653589793238462643383279502884197

double Cos(double x) {
  const double adjust = 2.0 * PI * floorl(x / (2.0 * PI));
  x -= adjust;
  double n =
      5678043342594608587418174133135360000.0 +
      x * x * (-2766947970816677844675767800512000000.0 +
      x * x * (201009028104053320861704991168972800.0 +
      x * x * (-5111523211284970219456401196512000.0 +
      x * x * (58952154443929839057881771856000.0 +
      x * x * (-338938990285362196790251245600.0 +
      x * x * (957049523024893733365255920.0 -
      1073709653319163169669575.0 * x * x))))));
  double d = 5678043342594608587418174133135360000.0 +
             x * x * (72073700480626449033319266055680000.0 +
             x * x * (460739069591187569274035316172800.0 +
             x * x * (1946779310366782428666671942400.0 +
             x * x * (5956981238166831424568016000.0 +
             x * x * (13425143673825939828558240.0 +
             x * x * (21053833464601381951920.0 +
             18164636218281257111.0 * x * x))))));
   return n / d;
}


int main(void) {
  printf("x        Cos             Library         Difference\n");
  printf("-        ---             -------         ----------\n");
  for (double r = -2.0 * PI; r <= 2.0 * PI; r += PI / 16) {
    printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", r, Cos(r), cos(r),
           Cos(r) - cos(r));
  }
}

