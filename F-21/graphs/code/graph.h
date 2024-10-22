#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct Graph Graph;

// Constructor for a graph with dynamically sized adjacency matrix.
// Performs no error checking with memory allocation.
Graph *graph_create(uint32_t vertices);

// Adds an edge <i, j> of weight k.
// Does not check if vertices are valid.
void graph_add_edge(Graph *g, uint32_t i, uint32_t j, uint32_t k);

// Checks if edge <i, j> exists.
// Does not check if vertices are valid.
bool graph_has_edge(Graph *g, uint32_t i, uint32_t j);

// Returns number of vertices in graph.
uint32_t graph_vertices(Graph *g);
