#include "pq.h"

#include "node.h"

#include <math.h>
#include <stdio.h> // only used for debugging
#include <stdlib.h>

// Credit to Prof. Long from assignment 3
// Returns 1 if x is more frequent than y,
// -1 if not, and 0 otherwise.
//
// x: first node to be compared
// y: second node to be compared
int cmp(Node x, Node y) {
    if (y.frequency < x.frequency) {
        return 1;
    } else if (y.frequency < x.frequency) {
        return -1;
    } else {
        return 0;
    }
}

// Credit to Prof. Long from assignment 3
// Swaps the pointers of the nodes.
//
// x: first node to be swapped
// y: second node to be swapped
void swap(Node *x, Node *y) {
    Node temp = *x;
    *x = *y;
    *y = temp;
}

// Finds the node with the lowest frequency and returns it.
//
// items: array of nodes
// first: parent node
// last: last valid index in the nodes array
uint32_t min_child(Node **items, uint32_t first, uint32_t last) {
    uint32_t left = 2 * first;
    uint32_t right = left + 1;
    if ((right <= last) && (cmp(*items[left - 1], *items[right - 1]) == 1)) {
        return right; // right is the smallest child
    }
    return left; // left is the smallest child
}

// Fixes a min heap by swapping a parent and child node if
// the child node is smaller than the parent.
//
// items: array of nodes
// first: parent node
// last: last valid index in the nodes array
void fix_heap(Node **items, uint32_t first, uint32_t last) {
    bool found = false;
    uint32_t mother = first;
    uint32_t small = min_child(items, mother, last);

    while ((mother <= floor(last / 2.0)) && !(found)) {
        if (cmp(*items[mother - 1], *items[small - 1]) == 1) { // smallest child < mother
            swap(items[mother - 1], items[small - 1]); // swap them
            mother = small;
            small = min_child(items, mother, last); // check the next children
        } else {
            found = true;
        }
    }
    return;
}

void build_heap(Node **items, uint32_t first, uint32_t last) {
    for (uint32_t father = floor(last / 2.0); father > first - 1; father -= 1) {
        fix_heap(items, father, last);
    }
    return;
}

// Declaration for PriorityQueue struct
struct PriorityQueue {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

// Creates and returns a PriorityQueue with a specified capacity,
// the top set to 0, and dynamically allocates memory for the
// PriorityQueue and array of nodes.
//
// capacity: maximum capacity for the PriorityQueue
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    if (pq) {
        pq->top = 0;
        pq->capacity = capacity;
        pq->items = (Node **) calloc(capacity, sizeof(Node *));
        if (!pq->items) {
            free(pq);
            pq = NULL;
        }
    }
    return pq;
}

// Frees memory used by the PriorityQueue and nodes array
// and dereferences the pointer to NULL before returning.
//
// q: PriorityQueue that is to be deleted
void pq_delete(PriorityQueue **q) {
    if (*q && (*q)->items) {
        free((*q)->items);
        free(*q);
        *q = NULL;
    }
    return;
}

// Returns true if the PriorityQueue is empty, false otherwise.
//
// q: PriorityQueue that is checked
bool pq_empty(PriorityQueue *q) {
    return (q->top == 0);
}

// Returns true if the PriorityQueue is full, false otherwise.
//
// q: PriorityQueue that is checked
bool pq_full(PriorityQueue *q) {
    return (q->top == q->capacity);
}

// Returns the number of items in the PriorityQueue
//
// q: PriorityQueue that is checked
uint32_t pq_size(PriorityQueue *q) {
    return q->top;
}

// Enqueues an node into the PriorityQueue if it is not full.
// Rebuilds the min heap once the node is added.
// Returns false if full and true otherwise.
//
// q: enqueue into this PriorityQueue
// n: node that is enqueued
bool enqueue(PriorityQueue *q, Node *n) {
    if (pq_full(q)) {
        return false;
    }
    q->items[q->top] = n;
    q->top++;
    build_heap(q->items, 1, pq_size(q));
    return true;
}

// Dequeues an node from the PriorityQueue if it is not empty.
// Rebuilds the min heap once the node is dequeued.
// Returns false if empty and true otherwise.
//
// q: dequeue from this PriorityQueue
// n: node that is dequeued
bool dequeue(PriorityQueue *q, Node **n) {
    if (pq_empty(q)) {
        return false;
    }
    swap(q->items[0], q->items[pq_size(q) - 1]);
    *n = q->items[pq_size(q) - 1];
    q->top--;
    build_heap(q->items, 1, pq_size(q));
    return true;
}

// Debug function for pq
void pq_print(PriorityQueue *q) {
    for (uint32_t i = 0; i < q->top; i++) {
        printf("Item %d: %lu ", i, (q->items[i])->frequency);
    }
    printf("\n");
}
