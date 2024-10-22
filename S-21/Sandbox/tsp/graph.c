#include "graph.h"
#include "vertices.h"
#include <stdlib.h>

struct Graph {
    uint32_t vertices;
    bool undirected;
    bool visited[VERTICES];
    uint32_t matrix[VERTICES][VERTICES];
};

Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *)calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    return G;
}

void graph_delete(Graph **G) {
    free(*G);
    *G = NULL;
}

uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}

static bool inbounds(Graph *G, uint32_t vertex) {
    return vertex < G->vertices; // Only need to check upper bound since unsigned >= 0.
}

bool graph_add_edge(Graph *G, uint32_t from, uint32_t to, uint32_t weight) {
    if (inbounds(G, from) && inbounds(G, to)) {
        G->matrix[from][to] = weight;
        if (G->undirected) {
            G->matrix[to][from] = weight;
        }
    }
    return false;
}

bool graph_has_edge(Graph *G, uint32_t from, uint32_t to) {
    return (inbounds(G, from) && inbounds(G, to)) ? G->matrix[from][to] > 0 : false;
}

uint32_t graph_edge_weight(Graph *G, uint32_t from, uint32_t to) {
    return (inbounds(G, from) && inbounds(G, to)) ? G->matrix[from][to] : 0;
}

bool graph_visited(Graph *G, uint32_t vertex) {
    return inbounds(G, vertex) ? G->visited[vertex] : false;
}

void graph_mark_visited(Graph *G, uint32_t vertex) {
    G->visited[vertex] = true;
}

void graph_mark_unvisited(Graph *G, uint32_t vertex) {
    G->visited[vertex] = false;
}
