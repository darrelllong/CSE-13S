#include "gs.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct GoodSpeak {
  char *oldspeak;
  char *newspeak;
};

GoodSpeak *gs_create(char *oldspeak, char *newspeak) {
  GoodSpeak *g = (GoodSpeak *)malloc(sizeof(GoodSpeak));
  check(g, "GoodSpeak not allocated!\n");

  g->oldspeak = strndup(oldspeak, strlen(oldspeak));
  check(g, "Oldspeak not allocated!\n");
  str_lowercase(g->oldspeak);

  if (newspeak) {
    g->newspeak = strndup(newspeak, strlen(newspeak));
    check(g, "Newspeak not allocated!\n");
    str_lowercase(g->newspeak);
  } else {
    g->newspeak = NULL;
  }

  return g;
}

void gs_delete(GoodSpeak *g) {
  if (g->newspeak) {
    free(g->newspeak);
    g->newspeak = NULL;
  }

  free(g->oldspeak);
  g->oldspeak = NULL;
  free(g);
  g = NULL;
  return;
}

char *gs_oldspeak(GoodSpeak *g) {
  return g->oldspeak;
}

char *gs_newspeak(GoodSpeak *g) {
  return g->newspeak;
}
