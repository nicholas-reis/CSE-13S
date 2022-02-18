#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "aeisqhr:n:p:"

static const uint32_t bit_mask = 0x3fffffff; // bit_mask for random numbers
typedef enum { HEAP, INSERTION, SHELL, QUICK } Sorts;
typedef enum { SEED = 5, SIZE = 6, ELEMENTS = 7, HELP = 8 } Options;
const char *names[] = { "Heap Sort", "Insertion Sort", "Shell Sort", "Quick Sort" };
static uint32_t elements = 100; // default number of elements to print
static uint32_t size = 100; // default size of array
static uint32_t seed = 13371453; // default seed

// prints out the help information.
// returns void.
void print_help() {
    printf("SYNOPSIS\n   A collection of comparison-based sorting algorithms.\n\n");
    printf("USAGE\n   ./sorting [haeisqn:p:r:] [-n length] [-p elements] [-r seed]\n\n");
    printf("OPTIONS\n   -h%14sdisplay program help and usage.\n", " ");
    printf("   -a%14senable all sorts.\n", " ");
    printf("   -e%14senable Heap Sort.\n", " ");
    printf("   -i%14senable Insertion Sort.\n", " ");
    printf("   -s%14senable Shell Sort.\n", " ");
    printf("   -q%14senable Quick Sort.\n", " ");
    printf("   -n length%7sspecify number of array elements (default: 100).\n", " ");
    printf("   -p elements%5sspecify number of eleemnts to print (default: 100).\n", " ");
    printf("   -r seed%9sspecify random seed (default: 13371453).\n", " ");
    return;
}

// resets the sorted array back to its initial order.
// returns void.
//
// A: sorted array
// B: contains initial array order
void reset_array(uint32_t *A, uint32_t *B, uint32_t length) {
    for (uint32_t i = 0; i < length; i += 1) {
        A[i] = B[i];
    }
    return;
}

int main(int argc, char **argv) {
    Stats stats;
    stats.moves = 0;
    stats.compares = 0;
    Set s = empty_set();

    int opt = 0;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a': s = complement_set(s); break; // set all bits of the set to 1
        case 'e': s = insert_set(HEAP, s); break; // insert specific option into set
        case 'i': s = insert_set(INSERTION, s); break;
        case 's': s = insert_set(SHELL, s); break;
        case 'q': s = insert_set(QUICK, s); break;
        case 'r':
            s = insert_set(SEED, s);
            seed = atoi(optarg);
            break; // atoi(optarg) for int input
        case 'n':
            s = insert_set(SIZE, s);
            size = atoi(optarg);
            break;
        case 'p':
            s = insert_set(ELEMENTS, s);
            elements = atoi(optarg);
            break;
        case 'h': s = insert_set(HELP, s); break;
        case '?': break;
        default: break;
        }
    }
    /*
    if (member_set(HELP, s) || s == empty_set()) {
        if (s == empty_set()) {
            printf("Select at least one sort to perform.\n");
        }
        print_help(); // print help when HELP is in set or no user input given
    }
*/
    //  srandom(seed); // for pseudorandom numbers
    /*
    uint32_t *A = (uint32_t *) calloc(size, sizeof(uint32_t)); // allocate memory for array A
    uint32_t *B = (uint32_t *) calloc(size, sizeof(uint32_t)); // allocate memory for array B
    for (uint32_t i = 0; i < size; i += 1) {
        uint32_t rand = random();
        A[i] = (bit_mask & rand); // each index is randomized and bit-masked
        B[i] = A[i]; // second array to store original values of the array
    }
*/

    srandom(seed);
    for (uint32_t i = 1000; i <= 50000; i += 1000) {
        uint32_t *A = (uint32_t *) calloc(i, sizeof(uint32_t));
        uint32_t *B = (uint32_t *) calloc(i, sizeof(uint32_t));
        for (uint32_t j = 0; j < i; j += 1) {
            uint32_t rand = random();
            A[j] = (bit_mask & rand);
            B[j] = A[j];
        }
        heap_sort(&stats, A, i);
        printf("%lu,", stats.moves); // heap moves
        reset_array(A, B, i);
        shell_sort(&stats, A, i);
        printf(" %lu,", stats.moves); // shell moves
        reset_array(A, B, i);
        insertion_sort(&stats, A, i);
        printf(" %lu,", stats.moves); // insert moves
        reset_array(A, B, i);
        quick_sort(&stats, A, i);
        printf(" %lu, %d\n", stats.moves, i); // quick moves

        free(A);
        free(B);
    }
    /*
	   
    heap_sort(&stats, A, size);
    
    reset_array(A, B); // need to reset the array to original values
    shell_sort(&stats, A, size);
    
    reset_array(A, B);
    insertion_sort(&stats, A, size);
    
    reset_array(A, B);
    quick_sort(&stats, A, size);
            
    printf("%s, %d elements, %lu moves, %lu compares\n", names[x], size, stats.moves,
                stats.compares);
            for (uint32_t i = 0; i < elements && i < size; i += 1) {
                if ((i != 0) && (i % 5 == 0)) {
                    printf("\n");
                }
                printf("%13" PRIu32, A[i]); // print out each number - 5 columns
            }
            printf("\n");
    }
    free(A); // free the memory
    free(B);
*/
    return 0;
}
