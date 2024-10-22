/*
** maze.c
**
** Find a path through a maze using a simple recursive depth-first search.
**
** DDEL
** Wed Nov  5 13:29:28 PST 2003
*/

#include <curses.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SLEEP 10000
#define DENSITY 30

#define WALL '#'
#define HALL ' '
#define DONE '!'
#define VISITED '.'
#define CURSOR '*'

#define X_MAZE 79
#define Y_MAZE 21

short maze[X_MAZE][Y_MAZE];

/*
** Valid moves are: up, right, left, down.
*/

#define N_MOVES 4

short xMove[] = {0, 1, -1, 0};
short yMove[] = {1, 0, 0, -1};

/*
** centum - return a random integer between 0 and 99.
*/

int centum() { return rand() % 100; }

/*
** mkMaze - Throw up random walls. This is not particularly elegant, but it
** will do for experimental purposes.
**
** n.b. The exit is on the bottom right.
*/

void mkMaze() {
  int i, j;

  for (i = 0; i < X_MAZE; i += 1)
    for (j = 0; j < Y_MAZE; j += 1)
      maze[i][j] = (centum() < DENSITY) ? WALL : HALL;

  maze[0][0] = HALL;
  maze[rand() % X_MAZE][rand() % Y_MAZE] = DONE;
}

/*
** printMaze - Print the maze in simple ASCII format.
*/

void printMaze() {
  int i, j;

  for (j = 0; j < Y_MAZE; j += 1)
    for (i = 0; i < X_MAZE; i += 1)
      mvaddch(j, i, maze[i][j]);
  refresh();
}

void place(int x, int y) {
  maze[x][y] = CURSOR;
  mvaddch(y, x, CURSOR);
  usleep(SLEEP);
  refresh();
}

void retreat(int x, int y) {
  maze[x][y] = VISITED;
  mvaddch(y, x, VISITED);
  refresh();
}

#define VALID(X, Y) ((X >= 0) && (X < X_MAZE) && (Y >= 0) && (Y < Y_MAZE))
#define VACANT(X, Y) (maze[X][Y] == HALL)
#define TERMINUS(X, Y) (maze[X][Y] == DONE)

int findPath(int x, int y) {
  usleep(1); /* Delay long enough to be able to see it. */

  if (TERMINUS(x, y)) {
    return TRUE;
  } else if (VALID(x, y) && VACANT(x, y)) {
    int i;

    place(x, y);
    for (i = 0; i < N_MOVES; i += 1) {
      if (findPath(x + xMove[i], y + yMove[i])) {
        return TRUE;
      }
    }
    retreat(x, y); /* Fail: back up and try again. */
  }
  return FALSE;
}

void die() {
  endwin();
  exit(0);
}

int main() {
  int i;

  (void)signal(SIGINT, die);

  srand(time(NULL));
  mkMaze();

  (void)initscr();
  (void)nonl();

  printMaze();
  if (!findPath(0, 0)) {
    mvaddstr(Y_MAZE + 1, 0, "no solution.\n");
    refresh();
  }

  (void)getchar();
  (void)endwin();

  exit(0);
}
