#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int moves = 0;

void move(int x, char from, char to, char neigh) {
  if (x == 1) // Base case
  {
    printf("Move disk 1 from peg %c to peg %c", from, to);
    moves++;
    return;
  }
  move(x - 1, from, neigh, to);
  moves++;
  printf("\nMove disk %d from peg %c to peg %c\n", x, from, to);
  move(x - 1, neigh, to, from);
}

int calculate_min_moves(int x)
{
  return (1 << x) - 1;
}

int main(int argc, char **argv) {
  int flag, disks = 10, min_moves;
  while ((flag = getopt(argc, argv, "n:")) != -1) {
    switch (flag) {
    case 'n': {
      disks = atoi(optarg);
      break;
    }
    }
  }
  move(disks, 'A', 'B', 'C');
  min_moves = calculate_min_moves(disks);
  printf("\n\nMinimum amount of moves required: %d", min_moves);
  printf("\nNumber of moves used: %d\n", moves);
  return 0;
}
