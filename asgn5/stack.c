#include "stack.h"

#include "node.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h> // only used for debugging
#include <stdlib.h>

// Credit to Prof. Long
struct Stack {
    uint32_t top; // Index of next empty spot
    uint32_t capacity; // Number of items that can be pushed to Stack
    Node **items; // Nodes array
};

// Credit to Prof. Long from assignment 4
// Creates and returns a Stack with a specified capacity,
// the top of the stack set to 0, and dynamically allocates
// enough memory for the stack and nodes array.
//
// capacity: how many items can be pushed to the Stack
Stack *stack_create(uint32_t capacity) {
    // dynamically allocates memory for the Stack using malloc
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        // sets top of the Stack to 0 and capacity to capacity
        s->top = 0;
        s->capacity = capacity;
        // dynamically allocates memory for items using calloc
        s->items = (Node **) calloc(capacity, sizeof(Node *));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

// Credit to Prof. Long from assignment 4
// Frees memory used by Stack *s and the nodes array and dereferences
// pointer to NULL before returning.
//
// s: Stack that is to be deleted
void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

// Returns true if the stack is empty and false otherwise.
//
// s: Stack that is being checked
bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}

// Returns true if the stack is full and false otherwise.
//
// s: Stack that is being checked
bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}

// Returns the number of nodes in the stack.
//
// s: Stack that is being checked
uint32_t stack_size(Stack *s) {
    return s->top;
}

// Pushes a node into the stack if the stack is not full.
// Returns false if it is at capacity, returns true otherwise.
//
// s: push node onto this Stack
// n: node that is pushed to stack
bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = n;
    s->top++;
    return true;
}

// Pops a node out of the stack if the stack is not empty and
// sets the value in the memory n is pointing to as the popped node.
// Returns false if the stack is empty and true to indicate success.
//
// s: pop item out of this Stack
// n: move popped item to memory location of *n
bool stack_pop(Stack *s, Node **n) {
    if (stack_empty(s)) {
        return false;
    }
    s->top--;
    // Dereference n
    *n = s->items[s->top];
    return true;
}

// Debug function for stack
void stack_print(Stack *s) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        printf("Node %d: %lu ", i, (s->items[i])->frequency);
    }
    printf("\n");
}
