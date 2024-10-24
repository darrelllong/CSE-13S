void crank(n, w, s) float w[], *s;
int n;
{
  int j = 1, ji, jt;
  float t, rank;

  *s = 0.0;
  while (j < n) {
    if (w[j + 1] != w[j]) {
      w[j] = j;
      ++j;
    } else {
      for (jt = j + 1; jt <= n; jt++)
        if (w[jt] != w[j])
          break;
      rank = 0.5 * (j + jt - 1);
      for (ji = j; ji <= (jt - 1); ji++)
        w[ji] = rank;
      t = jt - j;
      *s += t * t * t - t;
      j = jt;
    }
  }
  if (j == n)
    w[n] = n;
}
