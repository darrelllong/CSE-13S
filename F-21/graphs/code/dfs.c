#include "set.h"
#include "stack.h"
#include "graph.h"
#include <stdio.h>
#include <inttypes.h>

void dfs(Graph *g, uint32_t v) {
    Set visited = set_empty();                  // Track where we've been.
    Stack *s = stack_create(graph_vertices(g)); // Capacity of stack is number of vertices.
    stack_push(s, v);

    while (!stack_empty(s)) {
        stack_pop(s, &v);
        visited = set_insert(visited, v);

        for (uint32_t u = 0; u < graph_vertices(g); u += 1) {
            if (!set_member(visited, u) && graph_has_edge(g, v, u)) {
                stack_push(s, u);
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
    dfs(g, 0);
    return 0;
}
