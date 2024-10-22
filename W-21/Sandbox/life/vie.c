#include <sys/types.h>
#include <sys/stat.h>
#include <curses.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Stay in the cellular universe:
// Count all positions that are not on the grid as zero.

static inline int valid(int **a, int x, int y, int n, int m) {
  return (x >= 0 && x < n && y >= 0 && y < m) ? a[x][y] : 0;
}

// Count all of the neighbors: {-1, 0, 1} x {-1, 0, 1} - {0, 0}.

int neighbors(int **a, int x, int y, int n, int m) {
  int sum = 0;
  for (int i = x - 1; i <= x + 1; i += 1) {
    for (int j = y - 1; j <= y + 1; j += 1) {
      sum += valid(a, i, j, n, m);
    }
  }
  return sum - a[x][y]; // Do not count self
}

// Count all of the neighbors: {-1, 0, 1} x {-1, 0, 1} - {0, 0}, but do it on a torus.

int voisins(int **a, int x, int y, int n, int m) {
  int sum = 0;
  for (int i = x - 1; i <= x + 1; i += 1) {
    for (int j = y - 1; j <= y + 1; j += 1) {
      sum += valid(a, (n + i) % n, (m + j) % m, n, m);
    }
  }
  return sum - a[x][y]; // Do not count self
}

// Dynamically allocate the matrix: that means we will take advantage of the equivalence of
// pointers with arrays in C.

int **makeMatrix(int n, int m) {
  int **t = calloc(n, sizeof(int *)); // Allocate a column of row pointers
  for (int i = 0; i < n; i += 1) {
    t[i] = calloc(m, sizeof(int)); // Allocate each row
  }
  return t;
}

// Free the dynamic matrix: this is done in the reverse order of allocation.

void freeMatrix(int **a, int n) {
  for (int i = 0; i < n; i += 1) {
    free(a[i]); // Free each row
  }
  free(a); // Free the column of row pointers
  return;
}

// Print the matrix representing the universe. It will always print flat, whether or not it is
// being treated as a torus.

void printMatrix(FILE *outFile, int **a, int n, int m) {
  for (int i = 0; i < n; i += 1) {
    for (int j = 0; j < m; j += 1) {
      fprintf(outFile, "%c", a[i][j] ? '1' : '0');
    }
    fprintf(outFile, "\n");
  }
  return;
}

// Load the matrix from a file. This really should have better error handling.

void loadMatrix(FILE *inFile, int **a, int n, int m) {
  int x, y;
  while (fscanf(inFile, "%d %d", &x, &y) != EOF) {
    x = abs(x); y = abs(y);
    if (x >= 0 && x < n && y >= 0 & y < m) {
      a[x][y] = 1;
    }
    else {
      fprintf(stderr, "Point (%d, %d) not on the grid\n", x, y); exit(1);
    }
  }
  return;
}

// Evolve a single step in time. This requires having a before and after pictures since in this
// universe time ticks in discrete steps.
//
// For efficiency we have separated the regular from the toroidal calculation to avoid branches
// in the loops.

void evolve(int **old, int **new, int n, int m, bool torus) {
  for (int i = 0; i < n; i += 1) {
    for (int j = 0; j < m; j += 1) {
      int count = torus ? voisins(old, i, j, n, m) : neighbors(old, i, j, n, m);
      if (old[i][j] && (count == 2 || count == 3)) {
        new[i][j] = 1; // New life
      } else if (!old[i][j] && count == 3) {
        new[i][j] = 1; // Survival
      } else {
        new[i][j] = 0; // Death from loneliness or overcrowding
      }
    }
  }
  return;
}

// Print the matrix representing the universe. It will always print flat, whether or not it is
// being treated as a torus. But this time, do it using -lncurses for animation!

#define DELAY 50000

void display(int **a, int n, int m) {
  clear(); // Clear the screen (easier than individual erasures)
  for (int i = 0; i < n; i += 1) {
    for (int j = 0; j < m; j += 1) {
      if (a[i][j]) {
        mvprintw(i, j, "o");
      }
    }
  }
  refresh(); // Redraw the screen
  usleep(DELAY);
  return;
}

#define OPTIONS "tsn:i:o:"
#define ITERATIONS 50

int main(int argc, char **argv) {
  int opt = 0;
  bool torus = false, silent = false;
  int iterations = ITERATIONS;
  FILE *inFile = stdin, *outFile = stdout;

  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 't':
      torus = true;
      break;
    case 's':
      silent = true;
      break;
    case 'n':
      iterations = atoi(optarg);
      break;
    case 'i':
      inFile = fopen(optarg, "r");
      break;
    case 'o':
      outFile = fopen(optarg, "w");
      break;
    default:
      return 1;
    }
  }

  int n, m;
  fscanf(inFile, "%d %d\n", &n, &m); // Get the dimensions, again better error handling is needed.

  int **now = makeMatrix(n, m), **then = makeMatrix(n, m);

  loadMatrix(inFile, now, n, m);

  initscr(); noecho(); curs_set(FALSE); // Set up screen

  for (int i = 0; i < iterations; i += 1) {
    if (!silent) {
      display(then, n, m); // Populate the screen
    }
    evolve(now, then, n, m, torus);

    int **t = now; now = then; then = t; // Exchange the matrices so that then is now!
  }

  (void) getchar(); // Wait for the user to hit a key

  endwin(); // Close down curses

  printMatrix(outFile, now, n, m);

  freeMatrix(now, n); // Clean up your mess!
  freeMatrix(then, n);

  fclose(inFile);
  fclose(outFile);
  fchmod(fileno(outFile), 0644);

  return 0;
}
