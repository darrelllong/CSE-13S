#include "graph.h"
#include "set.h"
#include "queue.h"
#include <stdio.h>
#include <inttypes.h>

void bfs(Graph *g, uint32_t v) {
    Set visited = set_empty();                  // Capacity of queue is number of vertices.
    Queue *q = queue_create(graph_vertices(g)); // Capacity of queue is number of vertices.
    enqueue(q, v);

    while (!queue_empty(q)) {
        dequeue(q, &v);
        visited = set_insert(visited, v);

        for (uint32_t u = 0; u < graph_vertices(g); u += 1) {
            if (!set_member(visited, u) && graph_has_edge(g, v, u)) {
                enqueue(q, u);
            }
        }
    }
}

int main(void) {
    Graph *g = graph_create(8);
    graph_add_edge(g, 0, 1, 3); // <0, 1>, weight = 3
    graph_add_edge(g, 0, 2, 1); // <0, 2>, weight = 1
    graph_add_edge(g, 0, 3, 2); // <0, 3>, weight = 2
    graph_add_edge(g, 1, 4, 5); // <1, 4>, weight = 5
    graph_add_edge(g, 1, 5, 6); // <1, 5>, weight = 6
    graph_add_edge(g, 2, 6, 3); // <2, 6>, weight = 3
    graph_add_edge(g, 3, 7, 1); // <3, 7>, weight = 1
    bfs(g, 0);
    return 0;
}
