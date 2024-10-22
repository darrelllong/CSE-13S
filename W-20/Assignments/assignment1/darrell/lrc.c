#include <stdio.h>
#include <stdlib.h>

typedef enum faciem { LEFT, RIGHT, CENTER, PASS } faces;

faces die[] = {LEFT, RIGHT, CENTER, PASS, PASS, PASS};

const char *names[] = {"Dude",  "Walter", "Maude",    "Big Lebowski", "Brandt",
                       "Bunny", "Karl",   "Kieffer ", "Franz",        "Smokey"};

static inline int left(int pos, int players) { return ((pos + players - 1) % players); }

static inline int right(int pos, int players) { return ((pos + 1) % players); }

static inline int min(int x, int y) { return x < y ? x : y; }

int main(void) {
  int bank[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
  int seed, players;

  printf("Random seed: "); scanf("%d", &seed);

  srand(seed);

  printf("How many players? "); scanf("%d", &players);

  players = (players < 0 ? -players : players); // Only positive people may play!

  if (players < 1 || players > 10) {
    printf("Number of players must be from 1 to 10.\n");
    exit(1);
  }

  int pot = 0, activePlayers = players, player = 0;

  while (activePlayers > 1) {
    if (bank[player] > 0) {
      printf("%s rolls...", names[player]);

      for (int i = min(3, bank[player]); i > 0; i -= 1) {
        switch (die[rand() % 6]) {

        case LEFT:
          bank[player] -= 1;
          if (bank[left(player, players)] == 0) {
            activePlayers += 1;
          }
          bank[left(player, players)] += 1;
          printf(" gives $1 to %s", names[left(player, players)]);
          break;

        case RIGHT:
          bank[player] -= 1;
          if (bank[right(player, players)] == 0) {
            activePlayers += 1;
          }
          bank[right(player, players)] += 1;
          printf(" gives $1 to %s", names[right(player, players)]);
          break;

        case CENTER:
          pot += 1;
          bank[player] -= 1;
          printf(" puts $1 in the pot");
          break;

        default:
          printf(" gets a pass");
          break;
        }
      }
      printf("\n");
      if (bank[player] == 0) {
        activePlayers -= 1;
      }
    }
    player = (player + 1) % players;
  }

  int winner = 0;
  while (bank[winner] == 0) {
    winner += 1;
  }

  printf("%s wins the $%d pot with $%d left in the bank!\n", names[winner], pot,
         bank[winner]);

  return 0;
}
