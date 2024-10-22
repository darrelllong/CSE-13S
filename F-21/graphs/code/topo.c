#include "set.h"
#include "ll.h"
#include "graph.h"
#include <stdio.h>
#include <inttypes.h>

void toposort(Graph *g, uint32_t v, Set *visited, LinkedList *order) {
    *visited = set_insert(*visited, v);

    for (uint32_t u = 0; u < graph_vertices(g); u += 1) {
        if (!set_member(*visited, u) && graph_has_edge(g, v, u)) {
            toposort(g, u, visited, order);
        }
    }

    ll_insert(order, v);
}

int main(void) {
    Graph *g = graph_create(8);
    graph_add_edge(g, 0, 1, 5); // <0, 1>, weight = 5
    graph_add_edge(g, 1, 2, 2); // <0, 1>, weight = 2
    graph_add_edge(g, 1, 3, 2); // <0, 1>, weight = 2
    graph_add_edge(g, 3, 2, 3); // <0, 1>, weight = 3
    graph_add_edge(g, 2, 4, 4); // <0, 1>, weight = 4

    Set visited = set_empty();
    LinkedList *order = ll_create();
    toposort(g, 0, &visited, order);
    return 0;
}
