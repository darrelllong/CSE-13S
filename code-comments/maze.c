// BSD License
#include "LICENSE.h"

// maze.c
//
// Find a path through a maze using a simple recursive depth-first search.
//
// n.b. the worst case can be very bad, i.e. O(4**(x_maze*y_maze)), but most
// of the time we will get lucky.
//
// DDEL
// Wed Nov  5 13:29:28 PST 2003

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const int density = 30;

const char wall = 'X', hall = ' ', visited = '_', done = '!';

const int x_maze = 50;
const int y_maze = 30;

char maze[x_maze][y_maze];

// Valid moves are: down, left, right, up.

const int n_moves = 4;

short xMove[] = { 0, -1, 1, 0 };
short yMove[] = { -1, 0, 0, 1 };

// Graphics? We don't need to stinking graphics. We'll mark the moves with
// ASCII characters.

char mark[] = { '^', '<', '>', 'v' };

// centum - return a random integer between 0 and 99.

int centum() {
    return rand() % 100;
}

// mkMaze - Throw up random walls. This is not particularly elegant, but it
// will do for experimental purposes.
//
// n.b. The exit is on the bottom right.

void mkMaze() {
    int i, j;

    for (i = 0; i < x_maze; i += 1)
        for (j = 0; j < y_maze; j += 1)
            maze[i][j] = (centum() < density) ? wall : hall;

    maze[x_maze - 1][y_maze - 1] = hall;
}

// printMaze - Print the maze in simple ASCII format.

void printMaze() {
    int i, j;

    // Note: we want to print a row, not a column, and so the indices are reversed
    // from what one might normally expect to see.

    for (j = 0; j < y_maze; j += 1) {
        for (i = 0; i < x_maze; i += 1)
            putchar(maze[i][j]);
        printf("\n");
    }
}

static inline bool valid(int x, int y) {
    return (x >= 0) && (x < x_maze) && (y >= 0) && (y < y_maze);
}

static inline bool vacant(int x, int y) {
    return maze[x][y] == hall;
}

static inline bool terminus(int x, int y) {
    return (x == x_maze - 1) && (y == y_maze - 1);
}

int findPath(int x, int y) {
    if (terminus(x, y)) {
        return true;
    } else if (valid(x, y) && vacant(x, y)) {
        int i;

        for (i = 0; i < n_moves; i += 1) {
            maze[x][y] = mark[i];
            if (findPath(x + xMove[i], y + yMove[i])) {
                return true;
            }
        }
        maze[x][y] = visited; // Fail: back up and try again.
    }
    return false;
}

int main(void) {
    int i;

    srand(time(NULL));
    mkMaze();
    printMaze();

    for (i = 0; i < x_maze; i += 1)
        putchar('-');
    putchar('\n');

    if (findPath(0, 0)) {
        maze[x_maze - 1][y_maze - 1] = done;
        printMaze();
    } else
        printf("no solution.\n");

    exit(0);
}
