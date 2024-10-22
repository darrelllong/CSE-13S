#include <stdio.h>
#include "set.h"

int main(void)
{
  Set *s = set_create();
  set_insert(s, 1);
  set_insert(s, 3);
  set_insert(s, 4);
  set_insert(s, 5);
  set_insert(s, 7);
  set_insert(s, 9);
  set_insert(s, 11);
  set_insert(s, 13);
  set_insert(s, 15);
  set_insert(s, 1962);
  printf("s = ");
  set_print(s);

  Set *t = set_create();
  set_insert(t, 0);
  set_insert(t, 1);
  set_insert(t, 1);
  set_insert(t, 2);
  set_insert(t, 3);
  set_insert(t, 4);
  set_insert(t, 5);
  set_insert(t, 8);
  printf("t = ");
  set_print(t);

  Set *u = set_union(s, t);
  printf("s ∪ t = ");
  set_print(u);

  Set *v = set_intersect(s, t);
  printf("s ∩ t = ");
  set_print(v);

  Set *w = set_difference(s, t);
  printf("s - t = ");
  set_print(w);

  set_delete(s);
  set_delete(t);
  set_delete(u);
  set_delete(v);
  set_delete(w);

  return 0;
}
