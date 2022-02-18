#include "names.h"

#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SEED 2021

typedef enum { SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER } Position;
const Position pig[7] = { SIDE, SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER, JOWLER };

int main(void) {

    int player_count = 0;
    printf("How many players? ");
    scanf("%d", &player_count);
    if ((player_count < 2) || (player_count > 10)) { // player_count must be between 2 and 10
        fprintf(stderr, "Invalid number of players. Using 2 instead.\n");
        player_count = 2; // set player_count to 2 if invalid number is input
    }

    long seed = 0;
    long max_uint = 4294967295;
    printf("Random seed: ");
    scanf("%ld", &seed);
    if ((seed < 0) || (seed > max_uint)) { // seed must be positive and less than max unsigned int
        fprintf(stderr, "Invalid random seed. Using 2021 instead.\n");
        seed = SEED; // set seed to SEED (2021) if invalid number is input
    }
    srandom(seed); // uses the seed to create a repeatable sequence of pseudorandom numbers

    int scores[player_count];
    for (int i = 0; i < player_count; i++) {
        scores[i] = 0; // initialize all scores to 0
    }

    int current_player = 0;
    for (int i = 0; scores[current_player] < 100; i++) { // runs until someone reaches 100 score
        if (i == 0) {
            printf("%s rolls the pig...", names[current_player]);
        }
        int roll = random() % 7; // rolls randomly between 0 and 6
        switch (roll) {
        case 0:
        case 1:
            printf(" pig lands on side\n");
            if ((current_player + 1) == player_count) {
                current_player = 0; // back to player 0
                printf("%s rolls the pig...", names[current_player]);
                break;
            }
            current_player += 1; // next player's turn
            printf("%s rolls the pig...", names[current_player]);
            break;
        case 2:
            printf(" pig lands on back");
            scores[current_player] += 10;
            break;
        case 3:
            printf(" pig lands upright");
            scores[current_player] += 10;
            break;
        case 4:
            printf(" pig lands on snout");
            scores[current_player] += 15;
            break;
        case 5:
        case 6:
            printf(" pig lands on ear");
            scores[current_player] += 5;
            break;
        default: puts("How did you manage to do this?"); break;
        }
    }
    printf("\n%s wins with %d points!", names[current_player], scores[current_player]);
    printf("\n");
}
