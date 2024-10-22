#include <stdio.h>
#include <stdbool.h>

#define n 8
#define nSqr n * n

int a[8] = {2, 1, -1, -2, -2, -1, 1, 2};
int b[8] = {1, 2, 2, 1, -1, -2, -2, -1};

int chess_board[n][n];

int count = 0;

int attempt(int i, int x, int y) {
  int k, u, v;
  bool quit;
  k = 0;
  do {
    count += 1;
    quit = false;
    u = x + a[k];
    v = y + b[k];
    if ((u < n && u >= 0) && (v < n && v >= 0)) // On the board?
      if (chess_board[u][v] == 0) { // Vacant?
        chess_board[u][v] = i;      // Place knight
        if (i < nSqr) {             // Done?
          quit = attempt(i + 1, u, v);
          if (!quit) {
            chess_board[u][v] = 0;
          }
        } else {
          quit = true;
        }
      }
    k += 1;
  } while (!quit && k < 8);
  return (quit);
}

int main(void) {
  for (int i = 0; i < n; i += 1) {
    for (int j = 0; j < n; j += 1) {
      chess_board[i][j] = 0;
    }
  }
  chess_board[0][0] = 1;
  if (attempt(2, 0, 0)) {
    for (int i = 0; i < n; i += 1) {
      for (int j = 0; j < n; j += 1) {
        printf("\t%d", chess_board[i][j]);
      }
      printf("\n");
    }
    printf("solution required %d tries.\n", count);
    return 0;
  } else {
    printf("no solution in %d tries.\n", count);
    return -1;
  }
}
