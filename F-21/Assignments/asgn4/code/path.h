#pragma once

#include "graph.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct Path Path;

Path *path_create(void);

void path_delete(Path **p);

bool path_push_vertex(Path *p, uint32_t v, Graph *G);

bool path_pop_vertex(Path *p, uint32_t *v, Graph *G);

uint32_t path_vertices(Path *p);

uint32_t path_length(Path *p);

void path_copy(Path *dst, Path *src);

void path_print(Path *p, FILE *outfile, char *cities[]);
