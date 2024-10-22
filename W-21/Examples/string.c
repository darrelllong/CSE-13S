#include <stdio.h>

int mystrcmp(char *s1, char *s2) {
  for (; (*s1 && *s2) && (*s1 == *s2); s1++, s2++)
    ;
  return *s1 - *s2;
}

int mystrcmp2(char s1[], char s2[]) {
  int i = 0;
  while ((s1[i] && s2[i]) && (s1[i] == s2[i])) {
    i++;
  }
  return s1[i] - s2[i];
}

int mystrncmp(char *s1, char *s2, int n) {
  for (int i = 0; (i < n) && (*s1 && *s2) && (*s1 == *s2); i++, s1++, s2++)
    ;
  return *s1 - *s2;
}

int mystrncmp2(char s1[], char s2[], int n) {
  int i = 0;
  while ((i < n) && (s1[i] && s2[i]) && (s1[i] == s2[i]))
    i++;
  return s1[i] - s2[i];
}

int main(void) {
  char *s1 = "abcdef";
  char *s2 = "abcd";

  int cmp = mystrcmp2(s1, s2);

  if (cmp > 0) {
    printf("\"%s\" > \"%s\"\n", s1, s2);
  } else if (cmp < 0) {
    printf("\"%s\" < \"%s\"\n", s1, s2);
  } else {
    printf("\"%s\" = \"%s\"\n", s1, s2);
  }

  int cmpn = mystrncmp(s1, s2, 6);

  if (cmpn > 0) {
    printf("\"%s\" > \"%s\"\n", s1, s2);
  } else if (cmpn < 0) {
    printf("\"%s\" < \"%s\"\n", s1, s2);
  } else {
    printf("\"%s\" = \"%s\"\n", s1, s2);
  }

  return 0;
}
