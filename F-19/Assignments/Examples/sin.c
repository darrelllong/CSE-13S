#include <math.h>
#include <stdio.h>

#define PI 3.141592653589793238462643383279502884197

double Sin(double x) {
  const double adjust = 2.0 * PI * floorl(x / (2.0 * PI));
  x -= adjust;
  double n =
    x * (133684890792694976170345029120000.0 +
    x * x * (-20494411297105391300731453977600.0 +
    x * x * (828214724158018950289951814400.0 +
    x * x * (-13571254880131693253451022080.0 +
    x * x * (104761946712028505958458400.0 +
    x * x * (-385435322571107751366060.0 + 
    x * x * 555846146649952654679.0))))));
  double d = 
    133684890792694976170345029120000.0 +
    x * x * (1786403835010438060992717542400.0 +
    x * x * (11907940053967159035862828800.0 +
    x * x * (51483089628027222496531200.0 +
    x * x * (155112126388519498117920.0 +
    x * x * (315769438704063484140.0 + 
    x * x * 348811434161499491.0)))));
 return n / d;
}

int main(void) {
printf("x        Sin             Library         Difference\n");
printf("-        ---             -------         ----------\n");
  for ( double r = -2.0 * PI; r <= 2.0 * PI; r += PI / 16) {
  printf("% 6.4lf\t% 10.8lf\t% 10.8lf\t% 12.10lf\n", r, Sin(r), sin(r), Sin(r)-sin(r));
  }
}
