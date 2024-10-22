#include "path.h"
#include "stack.h"
#include "vertices.h"
#include <inttypes.h>
#include <stdlib.h>

struct Path {
    Stack *vertices;
    uint32_t length;
};

Path *path_create(void) {
    Path *p = (Path *)malloc(sizeof(Path));
    p->vertices = stack_create(VERTICES);
    p->length = 0;
    return p;
}

void path_delete(Path **p) {
    stack_delete(&(*p)->vertices);
    free(*p);
    *p = NULL;
}

bool path_push_vertex(Path *p, uint32_t vertex, Graph *G) {
    if (stack_empty(p->vertices)) {
        stack_push(p->vertices, vertex);
        p->length = graph_edge_weight(G, START_VERTEX, vertex);
    } else {
        uint32_t prev_vertex = 0;
        stack_peek(p->vertices, &prev_vertex);
        if (!stack_push(p->vertices, vertex)) {
            return false;
        }
        p->length += graph_edge_weight(G, prev_vertex, vertex);
    }
    return true;
}

bool path_pop_vertex(Path *p, uint32_t *vertex, Graph *G) {
    if (stack_empty(p->vertices)) {
        return false;
    }
    stack_pop(p->vertices, vertex);
    uint32_t prev_vertex = 0;
    stack_peek(p->vertices, &prev_vertex);
    p->length -= graph_edge_weight(G, prev_vertex, *vertex);
    return true;
}

uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

uint32_t path_length(Path *p) {
    return p->length;
}

void path_copy(Path *dst, Path *src) {
    dst->length = src->length;
    stack_copy(dst->vertices, src->vertices);
}

void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %" PRIu32 "\n", p->length);
    fprintf(outfile, "Path: %s%s", cities[START_VERTEX],
            stack_size(p->vertices) ? " -> " : "");
    stack_print(p->vertices, outfile, cities);
}
