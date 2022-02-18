#include "graph.h"

#include "vertices.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Credit to Prof. Long
struct Graph {
    uint32_t vertices; // Number of vertices
    bool undirected; // Undirected?
    bool visited[VERTICES]; // Where have we gone?
    uint32_t matrix[VERTICES][VERTICES]; // Adjacency matrix
};

// Credit to Prof. Long
// Creates and returns a Graph with specified number of vertices and
// whether or not it is undirected. Dynamically allocates space for it.
//
// vertices: number of vertices in the graph
// undirected: boolean that is true if the graph is undirected
Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *) calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    return G;
}

// Credit to Prof. Long
// Frees memory used by Graph *G and dereferences pointer to NULL
// before returning.
//
// Graph **G: pointer to a pointer of the Graph, used to avoid
// use-after-free errors
void graph_delete(Graph **G) {
    free(*G);
    *G = NULL;
    return;
}

// Accessor function that returns the number of vertices in a graph.
//
// Graph **G: check this graph for number of vertices
uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}

// Manipulator function that adds edges between vertices, along with
// the weight.
// Returns true if the vertices are within bounds and the edges are
// added, and false otherwise.
//
// Graph *G: graph that is being manipulated
// i: one of the vertices
// j: the other vertex
// k: weight of the edge between them
bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    if ((0 <= i) && (i < G->vertices) && (0 <= j) && (j < G->vertices) && (k >= 0)) {
        G->matrix[i][j] = k;
        if (G->undirected) {
            G->matrix[j][i] = k;
        }
        return true;
    }
    return false;
}

// Returns true if the vertices are within bounds and there is an
// edge between i and j, false otherwise.
//
// Graph *G: graph being checked
// i: one of the vertices
// j: the other vertex
bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    if ((0 <= i) && (i < G->vertices) && (0 <= j) && (j < G->vertices) && G->matrix[i][j] > 0) {
        return true;
    }
    return false;
}

// Returns the weight of the edge between vertices i and j as
// long as they are within bounds and the edge exists.
//
// Graph *G: graph being checked
// i: one of the vertices
// j: the other vertex
uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if (graph_has_edge(G, i, j)) {
        return G->matrix[i][j];
    } else {
        return 0;
    }
}

// Returns true if v has been visited, false otherwise.
//
// Graph *G: graph being checked
// v: vertex that is checked
bool graph_visited(Graph *G, uint32_t v) {
    if (G->visited[v]) {
        return true;
    }
    return false;
}

// Marks v as visited if it is within bounds before returning.
//
// Graph *G: graph being checked
// v: vertex that is marked as visited (true)
void graph_mark_visited(Graph *G, uint32_t v) {
    if (v < G->vertices) {
        G->visited[v] = true;
    }
    return;
}

// Marks v as unvisited if it is within bounds before returning.
//
// Graph *G: graph being checked
// v: vertex that is marked as unvisited (false)
void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v < G->vertices) {
        G->visited[v] = false;
    }
    return;
}

// Debug function that prints out a graph
void graph_print(Graph *G) {
    for (uint32_t i = 0; i < G->vertices; i += 1) {
        for (uint32_t j = 0; j < G->vertices; j += 1) {
            printf("%u ", G->matrix[i][j]);
        }
        printf("\n");
    }
    return;
}
