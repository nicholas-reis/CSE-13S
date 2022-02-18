#include "stack.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Credit to Prof. Long
struct Stack {
    uint32_t top; // Index of next empty spot
    uint32_t capacity; // Number of items that can be pushed to Stack
    uint32_t *items; // Array of items of type uint32_t
};

// Credit to Prof. Long
// Creates and returns a Stack with a specified capacity,
// the top of the stack set to 0, and dynamically allocates
// enough memory for the maximum capacity.
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
        s->items = (uint32_t *) calloc(capacity, sizeof(uint32_t));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

// Credit to Prof. Long
// Frees memory used by Stack *s and (*s)->items and dereferences
// pointer to NULL before returning.
//
// Stack **s: pointer to a pointer of the Stack, used to avoid
// use-after-free errors
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
// Stack *s: Stack that is being checked
bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}

// Returns true if the stack is full and false otherwise.
//
// Stack *s: Stack that is being checked
bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}

// Returns the number of items in the stack.
//
// Stack *s: Stack that is being checked
uint32_t stack_size(Stack *s) {
    return s->top;
}

// Pushes an item into the stack if the stack is not full.
// Returns false if it is at capacity, returns true otherwise.
//
// Stack *s: push item into this Stack
// x: item that is pushed to stack
bool stack_push(Stack *s, uint32_t x) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = x;
    s->top++;
    return true;
}

// Pops an item out of the stack if the stack is not empty and
// sets the value in the memory x is pointing to as the popped item.
// Returns false if the stack is empty and true to indicate success.
//
// Stack *s: pop item out of this Stack
// x: move popped item to memory location of *x
bool stack_pop(Stack *s, uint32_t *x) {
    if (stack_empty(s)) {
        return false;
    }
    // Credit to Prof. Long for this line of code
    // Dereferences x to change the value it points to
    s->top--;
    *x = s->items[s->top];
    return true;
}

// Works similarly to stack_pop, but does not modify the contents of
// the stack, just finds the top value of it.
// Returns false if the stack is empty and true to indicate success.
//
// Stack *s: peek into this Stack
// x: copy value on top of stack into the memory location of *x
bool stack_peek(Stack *s, uint32_t *x) {
    if (stack_empty(s)) {
        return false;
    }
    *x = s->items[s->top - 1];
    return true;
}

// Makes the stack dst a copy of the stack src. This means every item
// of src is copied into the items of dst, and they have the same top.
// The function then returns.
//
// Stack *dst: destination Stack
// Stack *src: source Stack
void stack_copy(Stack *dst, Stack *src) {
    for (uint32_t i = 0; i < src->capacity; i += 1) {
        dst->items[i] = src->items[i];
    }
    dst->top = src->top;
    return;
}

// Credit to Prof. Long
// Prints out everything in the stack to outfile with fprintf()
// before returning.
//
// Stack *s: Stack that is printed out
// *outfile: the output file where the stack is printed to
// *cities: the names of all the cities
void stack_print(Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
    return;
}
