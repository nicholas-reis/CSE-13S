#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS "hvui:o:"

static uint32_t recursive_calls = 0;

// Uses depth-first search to find paths that pass through all vertices and
// that also have an edge from the last vertex to the first one. Counts the
// number of recursive calls. Keeps track of the shortest path and if verbose
// option is enabled, print out each Hamiltonian path as it is found.
//
// Graph *G: graph used to keep track of edges
// v: current vertex
// curr: current path
// shortest: shortest found path
// cities: array of city names
// outfile: destination output to print to
// verbose: is the verbose option enabled?
void dfs(
    Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[], FILE *outfile, bool verbose) {
    recursive_calls += 1;
    if (path_length(curr) >= path_length(shortest) && path_length(shortest) > 0) {
        return;
    }
    uint32_t origin;
    if (path_vertices(curr) == graph_vertices(G) - 1 && graph_has_edge(G, v, 0)) {
        path_push_vertex(curr, 0, G);
        if (verbose && path_length(curr) > 0) {
            fprintf(outfile, "Path length: %u\nPath: ", path_length(curr));
            path_print(curr, outfile, cities);
        }
        if (path_length(curr) < path_length(shortest) || path_length(shortest) == 0) {
            path_copy(shortest, curr);
        }
        path_pop_vertex(curr, &origin, G);
    }
    graph_mark_visited(G, v);
    for (uint32_t w = 0; w < graph_vertices(G); w += 1) {
        if (graph_has_edge(G, v, w) && !graph_visited(G, w)) {
            graph_mark_visited(G, w);
            path_push_vertex(curr, w, G);
            dfs(G, w, curr, shortest, cities, outfile, verbose);
            graph_mark_unvisited(G, w);
            path_pop_vertex(curr, &w, G);
        }
    }
}

// Prints out the usage information and then exits the program
void usage() {
    printf("SYNOPSIS\n"
           "  Traveling Salesman Problem using DFS.\n\n"
           "USAGE\n"
           "  ./tsp [-u] [-v] [-h] [-i infile] [-o outfile]\n\n"
           "OPTIONS\n"
           "  -u            Use undirected graph.\n"
           "  -v            Enable verbose printing.\n"
           "  -h            Program usage and help.\n"
           "  -i infile     Input containing graph (default: stdin)\n"
           "  -o outfile    Output of computed path (default: stdout)\n");
    exit(0);
}

int main(int argc, char **argv) {
    int opt = 0;
    FILE *input = stdin;
    FILE *output = stdout;
    bool help = false;
    bool verbose = false;
    bool undirected = false;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help = true; break;
        case 'v': verbose = true; break;
        case 'u': undirected = true; break;
        case 'i':
            // Read from given file if it exists
            if (access(optarg, F_OK) == 0) {
                input = fopen(optarg, "r");
            } else {
                fprintf(output, "Error: failed to open infile.\n");
                exit(0);
            }
            break;
        case 'o':
            // Write to given file
            output = fopen(optarg, "w");
            break;
        case '?': help = true; break;
        default: help = true; break;
        }
    }

    if (help) {
        usage(); // Print out usage to stdout
    }

    char **names = (char **) calloc(VERTICES, sizeof(char *)); // Allocate memory for cities

    uint32_t num_verts = 0;
    fscanf(input, "%" SCNu32 "\n", &num_verts); // scan number of vertices

    for (uint32_t i = 0; i < num_verts; i += 1) { // fgets() all city names
        char buf[1024];
        if (fgets(buf, 1024, input) == NULL) {
            printf("Error: Line is malformed.");
        } else {
            buf[strlen(buf) - 1] = '\0';
        }
        names[i] = strdup(buf); // copy buf into names array properly
    }

    Graph *G = graph_create(num_verts, undirected);
    uint32_t i, j, k; // vertices i and j, weight k
    while ((fscanf(input, "%" SCNu32 "%" SCNu32 "%" SCNu32 "\n", &i, &j, &k) != EOF)) {
        graph_add_edge(G, i, j, k); // add each given edge to the graph
    }

    Path *curr = path_create();
    Path *shortest = path_create();

    dfs(G, START_VERTEX, curr, shortest, names, output, verbose); // find Hamiltonian paths
    if (path_length(shortest) > 0) {
        fprintf(output, "Path length: %u\nPath: ", path_length(shortest));
        path_print(shortest, output, names);
        fprintf(output, "Total recursive calls: %u\n", recursive_calls);
    } else {
        fprintf(output, "There's nowhere to go.\n");
    }

    // delete graph and anything allocated to stack
    graph_delete(&G);
    path_delete(&shortest);
    path_delete(&curr);
    for (uint32_t i = 0; i < num_verts; i += 1) {
        free(names[i]);
    }
    free(names);
}
