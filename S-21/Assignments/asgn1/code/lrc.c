#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static inline int left(int pos, int players) { return ((pos + players - 1) % players); }

static inline int right(int pos, int players) { return ((pos + 1) % players); }

typedef enum faciem { LEFT, RIGHT, CENTER, PASS } faces;
faces die[] = {LEFT, RIGHT, CENTER, PASS, PASS, PASS};

const char *names[] = {"Dude",  "Walter", "Maude",    "Big Lebowski", "Brandt",
                       "Bunny", "Karl",   "Kieffer ", "Franz",        "Smokey"};

int main(void) {
  int bank[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
  int pot = 0, seed, players, playersTurn = 0, winner;
  bool endGame = false;

  printf("Random seed: ");
  scanf("%d", &seed);
  srand(seed);

  printf("How many players? ");
  scanf("%d", &players);

  while (!endGame) {
    int playersCurrentMoney = bank[playersTurn], amountOfRolls;

    if (playersCurrentMoney > 0) {
      printf("%s rolls...", names[playersTurn]);
    }

    for (amountOfRolls = 0;
         amountOfRolls < playersCurrentMoney && amountOfRolls < 3;
         amountOfRolls++) {
      int roll = rand() % 6;
      switch (die[roll]) {
      case LEFT:
        bank[playersTurn]--;
        bank[left(playersTurn, players)]++;
        printf(" gives $1 to %s", names[left(playersTurn, players)]);
        break;

      case RIGHT:
        bank[playersTurn]--;
        bank[right(playersTurn, players)]++;
        printf(" gives $1 to %s", names[right(playersTurn, players)]);
        break;
      case CENTER:
        pot++;
        bank[playersTurn]--;
        printf(" puts $1 in the pot");
        break;
      default:
        printf(" gets a pass");
      }
    }
    if (amountOfRolls > 0) {
      printf("\n");
    }

    if (playersTurn < (players - 1)) {
      playersTurn++;

    }

    else if (playersTurn == (players - 1)) {
      playersTurn = 0;
    }

    int j;
    for (j = 0; j < players; j++) {
      if (bank[j] + pot == (players * 3)) {
        winner = j;
        endGame = true;
      }
    }
  }

  printf("%s wins the $%d pot with $%d left in the bank!", names[winner], pot,
         bank[winner]);
  printf("\n");

  return 0;
}
