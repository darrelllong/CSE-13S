#include "names.h"
#include <stdio.h>
#include <stdlib.h>

#define MIDNIGHT 10 // A roll is rand() % 6, so max is 5.

static inline int roll(void) {
  return rand() % 6;
}

static inline int left(int pos, int players) {
  return (pos + players - 1) % players;
}

static inline int right(int pos, int players) {
  return (pos + 1) % players;
}

int main(void) {
  int players = 0;
  printf("Number of players: ");
  scanf("%d", &players);
  if (players < 2 || players > 10) {
    printf("Invalid number of players.\n");
    return 1;
  }

  int seed = 0;
  printf("Random seed: ");
  scanf("%d", &seed);
  if (seed < 0) {
    printf("Invalid random seed.\n");
    return 1;
  }

  srand(seed);
  int active = players;
  int lives[10] = { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 };

  for (int r = 1; active > 1; r += 1) {
    printf("Round %d\n", r);
    int lowest = MIDNIGHT + 1;
    int roller = 0;

    for (int i = 0; i < players; i += 1) {
      if (lives[i] > 0) {
        int first = roll();
        int second = roll();
        printf(" - %s rolls %s...", names[i], rolls[first][second]);

        // Rolling Midnight either resurrects or sparkles the neighbor.
        if (first + second == MIDNIGHT) {
          if (lives[left(i, players)] == 0) {
            printf(" resurrects %s", names[left(i, players)]);
            active += 1;
          } else {
            printf(" sparkles %s", names[left(i, players)]);
          }
          lives[left(i, players)] += 1;

          if (lives[right(i, players)] == 0) {
            printf(" resurrects %s", names[right(i, players)]);
            active += 1;
          } else {
            printf(" sparkles %s", names[right(i, players)]);
          }
          lives[right(i, players)] += 1;
        }

        if (first + second < lowest) {
          lowest = first + second;
          roller = i;
        }

        printf("\n");
      }
    }

    printf("%s is forced to eat garlic!\n%s has ", names[roller], names[roller]);
    if (--lives[roller] == 0) {
      printf("died.\n");
      active -= 1;
    } else {
      printf("%d %s remaining.\n", lives[roller], lives[roller] == 1 ? "life" : "lives");
    }
  }

  for (int i = 0; i < players; i += 1) {
    if (lives[i]) {
      printf("%s wins the Garlic Games!\n", names[i]);
      break;
    }
  }

  return 0;
}
