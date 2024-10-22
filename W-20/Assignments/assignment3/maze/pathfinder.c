#include <ctype.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "set.h"
#include "stack.h"

#define A ('A' - 'A')
#define Z ('Z' - 'A')
#define SIZE ('Z' - 'A' + 1)

void zero(uint32_t n, bool m[n][n]) {
  for (uint32_t i = 0; i < n; i += 1) {
    for (uint32_t j = 0; j < n; j += 1) {
      m[i][j] = false;
    }
  }
}

#define LINE 1024

void fillGraph(FILE *f, bool directed, uint32_t n, bool m[n][n]) {
  char buffer[LINE];
  while (fscanf(f, "%s", buffer) != EOF) {
    if (strlen(buffer) > 1 && isalpha(buffer[0]) && isalpha(buffer[1])) {
      uint32_t x = toupper(buffer[0]) - 'A';
      uint32_t y = toupper(buffer[1]) - 'A';
      if (directed) {
        m[x][y] = true;
      } else {
        m[y][x] = m[x][y] = true;
      }
    }
  }
  return;
}

void printGraph(uint32_t n, bool m[n][n]) {
  printf("  A B C D E F G H I J K L M N O P Q R S T U V W X Y Z\n");
  for (uint32_t i = A; i <= Z; i += 1) {
    printf("%c", i + 'A');
    for (uint32_t j = A; j <= Z; j += 1) {
      printf(" %d", m[i][j] ? 1 : 0);
    }
    printf("\n");
  }
  return;
}

static set visited = 0;

static stack *path;

static uint32_t shortest = UINT32_MAX;
static uint32_t length = 0;

void DFS(uint32_t n, bool graph[n][n]) {
  visited = insertSet(n, visited);
  (void)push(path, &n);
  length += 1;
  if (n == Z) {
    shortest = length < shortest ? length : shortest;
    printf("Path: ");
    printStack(path);
  }
  for (uint32_t m = A; m <= Z; m += 1) {
    if (!memberSet(m, visited) && graph[n][m]) {
      DFS(m, graph);
    }
  }
  (void)pop(path, &n);
  visited = deleteSet(n, visited);
  length -= 1;
  return;
}

#define OPTIONS "i:dum"

int main(int argc, char **argv) {

  bool directed = false, printMaze = false;

  FILE *input = stdin;

  int c;
  while ((c = getopt(argc, argv, OPTIONS)) != -1) {
    switch (c) {
    case 'i':
      if ((input = fopen(optarg, "r")) == 0) {
        char buffer[LINE];
        snprintf(buffer, LINE, "%s: \"%s\"", argv[0], optarg);
        perror(buffer);
        exit(1);
      }
      break;
    case 'd':
      directed = true;
      break;
    case 'u':
      directed = false;
      break;
    case 'm':
      printMaze = true;
      break;
    default:
      break;
    }
  }

  bool graph[SIZE][SIZE];

  zero(SIZE, graph); fillGraph(input, directed, SIZE, graph);

  if (printMaze) {
    printGraph(SIZE, graph);
  }

  path = newStack();
  DFS(A, graph);
  delStack(path);

  if (shortest < UINT32_MAX) {
    printf("Shortest path %" PRIu32 "\n", shortest);
  } else {
    printf("No path found!\n");
  }

  fclose(input);

  return 0;
}
