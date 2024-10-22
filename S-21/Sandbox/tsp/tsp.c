#include "graph.h"
#include "path.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void usage(char *exec) {
    fprintf(stderr,
        "SYNOPSIS\n"
        "  Traveling Salesman Problem using DFS.\n"
        "\n"
        "USAGE\n"
        "  %s [-u] [-v] [-h] [-i infile] [-o outfile]\n"
        "\n"
        "OPTIONS\n"
        "  -u             Use undirected graph.\n"
        "  -v             Enable verbose printing.\n"
        "  -h             Program usage and help.\n"
        "  -i infile      Input containing graph (default: stdin)\n"
        "  -o outfile     Output of computed path (default: stdout)\n",
        exec);
}

#define OPTIONS "i:o:vuh"
#define KB      1024

static bool verbose = false;
static uint64_t calls = 0; // Track calls to dfs().

void dfs(Graph *G, uint32_t from, Path *curr, Path *shortest, char **cities, FILE *outfile) {
    calls += 1;

    // Reasons to consider more vertices:
    //  1) We haven't found a path yet.
    //  2) The current path isn't yet worse than a found path.
    if (path_length(shortest) == 0 || path_length(curr) < path_length(shortest)) {
        // Two cases for each vertex:
        //   1) It completes the Hamiltonian path and we update best path if needed.
        //   2) It doesn't and we keep searching unvisited vertices.
        if (path_vertices(curr) + 1 == graph_vertices(G) && graph_has_edge(G, from, START_VERTEX)) {
            path_push_vertex(curr, START_VERTEX, G);
            if (path_length(shortest) == 0 || path_length(curr) < path_length(shortest)) {
                path_copy(shortest, curr);
            }
            if (verbose) {
                path_print(curr, outfile, cities);
            }
            path_pop_vertex(curr, &from, G);
        } else {
            for (uint32_t to = 0; to < graph_vertices(G); to += 1) {
                // We only visit unvisited vertices.
                // Push it onto the path, mark as visited, undo when we return.
                if (graph_has_edge(G, from, to) && !graph_visited(G, to)) {
                    path_push_vertex(curr, to, G);
                    graph_mark_visited(G, to);
                    dfs(G, to, curr, shortest, cities, outfile);
                    path_pop_vertex(curr, &to, G);
                    graph_mark_unvisited(G, to);
                }
            }
        }
    }
}

int main(int argc, char **argv) {
    int opt = 0;
    FILE *infile = stdin;
    FILE *outfile = stdout;
    bool undirected = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i':
            if ((infile = fopen(optarg, "r")) == NULL) {
                fprintf(stderr, "Error: failed to open infile.\n");
                return EXIT_FAILURE;
            }
            break;
        case 'o':
            if ((outfile = fopen(optarg, "w")) == NULL) {
                fprintf(stderr, "Error: failed to open outfile.\n");
                return EXIT_FAILURE;
            }
            break;
        case 'u': undirected = true; break;
        case 'v': verbose = true; break;
        case 'h': usage(argv[0]); return EXIT_SUCCESS;
        default: usage(argv[0]); return EXIT_FAILURE;
        }
    }

    // Get the number of cities from the input.
    uint32_t vertices = 0;
    if (fscanf(infile, "%" SCNu32 "\n", &vertices) != 1 || vertices > VERTICES) {
        fprintf(stderr, "Error: malformed number of vertices.\n");
        return EXIT_FAILURE;
    }

    // Copy the city names down.
    char city[KB] = { 0 };
    char *cities[VERTICES]; // Holds city names.
    for (uint32_t i = 0; i < vertices; i += 1) {
        if (fgets(city, KB, infile) == NULL) {
            fprintf(stderr, "Error: malformed city name.\n");
            for (uint32_t j = 0; j < i; j += 1) {
                free(cities[j]);
            }
            return EXIT_FAILURE;
        }
        city[strlen(city) - 1] = '\0'; // Knock out extra newline.
        cities[i] = strdup(city);
    }

    // Make graph, load edges.
    // Each line is "from" vertex, "to" vertex, edge weight.
    int ret = 0;
    uint32_t i = 0, j = 0, w = 0;
    Graph *G = graph_create(vertices, undirected);
    while ((ret = fscanf(infile, "%" SCNu32 " %" SCNu32 " %" SCNu32 "\n", &i, &j, &w)) != EOF) {
        if (ret != 3) {
            fprintf(stderr, "Error: malformed edge.\n");
            for (uint32_t i = 0; i < vertices; i += 1) {
                free(cities[i]);
            }
            graph_delete(&G);
            return EXIT_FAILURE;
        }
        graph_add_edge(G, i, j, w);
    }

    // "And awaaaaay we go!" ---Rick Sanchez
    Path *curr = path_create();
    Path *shortest = path_create();
    graph_mark_visited(G, START_VERTEX);
    dfs(G, START_VERTEX, curr, shortest, cities, outfile);

    if (vertices <= 1) {
        fprintf(outfile, "There's nowhere to go.\n");
    } else {
        // If the shortest path has more than 1 vertex, a path was found.
        if (path_length(shortest) > 0) {
            path_print(shortest, outfile, cities);
        } else {
            fprintf(outfile, "No Hamiltonian path found.\n");
        }
        fprintf(outfile, "Total recursive calls: %" PRIu64 "\n", calls);
    }

    // Free the cities.
    for (uint32_t i = 0; i < vertices; i += 1) {
        free(cities[i]);
    }

    graph_delete(&G);
    path_delete(&curr);
    path_delete(&shortest);
    fclose(infile);
    fclose(outfile);
    return EXIT_SUCCESS;
}
