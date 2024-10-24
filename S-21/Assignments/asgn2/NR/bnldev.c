#include <math.h>

#define PI 3.141592654

float bnldev(pp, n, idum) float pp;
int n, *idum;
{
  int j;
  static int nold = (-1);
  float am, em, g, angle, p, bnl, sq, t, y;
  static float pold = (-1.0), pc, plog, pclog, en, oldg;
  float ran1(), gammln();

  p = (pp <= 0.5 ? pp : 1.0 - pp);
  am = n * p;
  if (n < 25) {
    bnl = 0.0;
    for (j = 1; j <= n; j++)
      if (ran1(idum) < p)
        bnl += 1.0;
  } else if (am < 1.0) {
    g = exp(-am);
    t = 1.0;
    for (j = 0; j <= n; j++) {
      t *= ran1(idum);
      if (t < g)
        break;
    }
    bnl = (j <= n ? j : n);
  } else {
    if (n != nold) {
      en = n;
      oldg = gammln(en + 1.0);
      nold = n;
    }
    if (p != pold) {
      pc = 1.0 - p;
      plog = log(p);
      pclog = log(pc);
      pold = p;
    }
    sq = sqrt(2.0 * am * pc);
    do {
      do {
        angle = PI * ran1(idum);
        y = tan(angle);
        em = sq * y + am;
      } while (em < 0.0 || em >= (en + 1.0));
      em = floor(em);
      t = 1.2 * sq * (1.0 + y * y) *
          exp(oldg - gammln(em + 1.0) - gammln(en - em + 1.0) + em * plog +
              (en - em) * pclog);
    } while (ran1(idum) > t);
    bnl = em;
  }
  if (p != pp)
    bnl = n - bnl;
  return bnl;
}

#undef PI
