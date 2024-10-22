#include "stack.h"
#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ALPHABET  26
#define START     0
#define TARGET(x) (x == ALPHABET - 1)
#define OPTIONS   "i:dum"

static uint32_t paths = 0;               // Counts number of valid paths.
static uint32_t shortest = UINT32_MAX;   // Keeps track of shortest path.

static bool found;                       // Does a path exist?
static bool visited[ALPHABET];           // Global visited array.
static bool maze[ALPHABET][ALPHABET];    // Global adjacency matrix (the maze).
static Stack *path = NULL;               // Global stack that holds the path.

typedef enum modes { UNDIRECTED, DIRECTED } modes;

uint32_t min(uint32_t x, uint32_t y) {
  return (x < y) ? x : y;
}

void print_matrix(void) {
  printf("  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z\n");

  for (uint32_t i = 0; i < ALPHABET; i += 1) {
    printf("%c ", i + 'A');

    for (uint32_t j = 0; j < ALPHABET; j += 1) {
      printf(maze[i][j] ? "1" : "0");

      if (j != ALPHABET - 1) {
        printf(" ");
      }
    }

    printf("\n");
  }

  return;
}

void dfs(uint32_t curr) {
  if (TARGET(curr)) {
    found = true;
    paths += 1;
    printf("Found path: ");
    stack_print(path);
    shortest = min(shortest, stack_size(path));
    return;
  }

  visited[curr] = true;

  for (uint32_t next = 0; next < ALPHABET; next += 1) {
    if (maze[curr][next] && !visited[next]) {
      stack_push(path, next);
      dfs(next);
      uint32_t temp;
      stack_pop(path, &temp);
      visited[next] = false;
    }
  }

  return;
}


int main(int argc, char **argv) {
  int c = 0;
  bool print = false;
  FILE *infile = stdin;
  modes mode = UNDIRECTED;

  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 'i':
      infile = fopen(optarg, "r");
      if (!infile) {
        perror("fopen");
        exit(-1);
      }
      break;
    case 'u':
      mode = UNDIRECTED;
      break;
    case 'd':
      mode = DIRECTED;
      break;
    case 'm':
      print = true;
      break;
    default:
      fprintf(stderr, "Usage: %s -i <input file>\n", argv[0]);
      exit(-1);
    }
  }

  char x_coord; uint32_t x = 0;
  char y_coord; uint32_t y = 0;

  while (fscanf(infile, "%c%c\n", &x_coord, &y_coord) != -1) {
    if (!isalpha(x_coord) || !isalpha(y_coord)) {
      fprintf(stderr, "Nodes must be between the range of A - Z!\n");
      exit(-1);
    }

    x = toupper(x_coord) - 'A';
    y = toupper(y_coord) - 'A';

    maze[x][y] = true;

    if (mode == UNDIRECTED) {
      maze[y][x] = true;
    }
  }

  if (print) {
    print_matrix();
  }

  path = stack_create();
  stack_push(path, START);

  dfs(START);

  if (found) {
    printf("Number of paths found: %" PRIu32 "\n", paths);
    printf("Length of shortest path found: %" PRIu32 "\n", shortest);
  } else {
    printf("No paths found!\n");
  }

  stack_delete(path);
  fclose(infile);
  return 0;
}
