#include "path.h"

#include "graph.h"
#include "stack.h"
#include "vertices.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Path {
    Stack *vertices; // Vertices of the path
    uint32_t length; // Length of the path
};

// Creates and returns a Path with a vertices Stack
// that stores up to VERTICES number of vertices and
// a length of 0
Path *path_create(void) {
    Path *p = (Path *) malloc(sizeof(Path));
    p->vertices = stack_create(VERTICES);
    p->length = 0;
    return p;
}

// Frees memory used by Path *p, the p->vertices stack,
// and dereferences pointer to NULL before returning.
//
// Path **p: pointer to a pointer of the Path, used to
// avoid use-after-free errors.
void path_delete(Path **p) {
    stack_delete(&(*p)->vertices);
    free(*p);
    *p = NULL;
    return;
}

// Pushes a vertex v onto the path and increases the length of
// the path by the edge weight between vertex v and the vertex
// on top of the stack. Returns true if the vertex is pushed
// successfully and false otherwise.
//
// Path *p: Path that is utilized
// v: vertex to be pushed to stack
// Graph *G: used to check the edge weight
bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    // Will store vertex on top of stack
    uint32_t vertex_i = 0;
    // If there is a vertex on top of the stack...
    if (stack_peek(p->vertices, &vertex_i)) {
        // Length increased by edge weight between top of stack and v
        p->length += graph_edge_weight(G, vertex_i, v);
    } else {
        // Length increased by edge weight between origin vertex and v
        p->length += graph_edge_weight(G, START_VERTEX, v);
    }
    if (stack_push(p->vertices, v)) {
        return true;
    }
    return false;
}

// Pops a vertex *v from the path and decreases the length of
// the path by the edge weight between vertex v and the vertex
// on top of the stack. Returns true if the vertex is popped
// successfully and false otherwise.
//
// Path *p: Path that is utilized
// *v: move popped item to memory location of *v
// Graph *G: used to check the edge weight
bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    // Pop the vertex and store in *v
    if (stack_pop(p->vertices, v)) {
        // Will store vertex on top of the stack
        uint32_t vertex_i = 0;
        if (stack_peek(p->vertices, &vertex_i)) {
            // length decreased by edge between vertex on top of the stack and v
            p->length -= graph_edge_weight(G, vertex_i, *v);
        } else {
            // length decreased by edge between start vertex and v
            p->length -= graph_edge_weight(G, 0, *v);
        }
        return true;
    }
    return false;
}
// Returns the number of vertices in the path
//
// Path *p: path that is being checked
uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

// Returns the length of the path
//
// Path *p: path that is being checked
uint32_t path_length(Path *p) {
    return p->length;
}

// Makes the destination Path a copy of the source Path by
// copying over the lengths along with the vertices stack
// before returning.
//
// Path *dst: destination path
// Path *src: source path
void path_copy(Path *dst, Path *src) {
    dst->length = src->length;
    stack_copy(dst->vertices, src->vertices);
    return;
}

// Prints out a path to outfile using stack_print() before
// returning.
//
// Path *p: path that is printed out
// outfile: output file location
// cities[]: character array of city names
void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "%s -> ", cities[0]);
    stack_print(p->vertices, outfile, cities);
    return;
}
