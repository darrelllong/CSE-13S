#include <math.h>
#include <stdio.h>

long double Sec(double x) {
  long double p = 1
                  + x * x
                        * (-6.919599825198853e7
                            + x * x
                                  * (-2.0346537754069692e6
                                      + x * x
                                            * (-29320.33825008159
                                                - (203174568493577 * x * x)
                                                      / 9.074884896e11)));
  long double q = 1
                  + x * x
                        * (-6.919599875198853e7
                            + x * x
                                  * (3.2563345392253965e7
                                      + x * x
                                            * (-1.8951600457683413e6
                                                + (151976976957241937 * x * x)
                                                      / 5.8986751824e12)));
  return p / q;
}

int main(void) {
  for (double x = 0; x < M_PI / 2; x += M_PI / 180) {
    printf("% 10.8Lf\t% 10.8lf\t% 10.8Lf\n", Sec(x), 1 / cos(x),
        1 / cos(x) - Sec(x));
  }
  return 0;
}
