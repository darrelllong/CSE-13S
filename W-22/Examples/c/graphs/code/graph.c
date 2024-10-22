#include "graph.h"
#include <stdlib.h>

struct Graph {
    uint32_t vertices; // Number of vertices in graph.
    uint32_t **matrix; // Adjacency matrix.
};

// Constructor for a graph with dynamically sized adjacency matrix.
// Performs no error checking with memory allocation.
Graph *graph_create(uint32_t vertices) {
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->vertices = vertices;
    g->matrix = (uint32_t **)calloc(vertices, sizeof(uint32_t *));
    for (uint32_t i = 0; i < vertices; i += 1) {
        g->matrix[i] = (uint32_t *)calloc(vertices, sizeof(uint32_t));
    }
    return g;
}

// Adds an edge <i, j> of weight k.
// Does not check if vertices are valid.
void graph_add_edge(Graph *g, uint32_t i, uint32_t j, uint32_t k) {
    g->matrix[i][j] = k;
}

// Checks if edge <i, j> exists.
// Does not check if vertices are valid.
bool graph_has_edge(Graph *g, uint32_t i, uint32_t j) {
    return g->matrix[i][j] > 0;
}

// Returns number of vertices in graph.
uint32_t graph_vertices(Graph *g) {
    return g->vertices;
}
