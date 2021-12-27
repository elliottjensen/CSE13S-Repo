#include "names.h"

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    // recieve num_players
    int num_players;
    printf("How many players? ");
    scanf("%d", &num_players);

    // check num_players size

    if (num_players < 2 || num_players > 10) {
        fprintf(stderr, "Invalid number of players. Using 2 instead.\n");
        num_players = 2;
    }

    // recieve seed
    int seed;
    printf("Random seed: ");
    scanf("%d", &seed);

    // check seed size
    if (seed <= 0) {
        fprintf(stderr, "Invalid random seed. Using 2021 instead.\n");
        seed = 2021;
    }

    // create dice
    //typedef enum { SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER } Position;
    //const Position pig[7] = { SIDE, SIDE, RAZORBACK, TROTTER, SNOUTER, JOWLER, JOWLER };

    // create scores
    int player_scores[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    // use seed
    srandom(seed);

    // game loop
    int i;
    int roll;

    i = 0;

    do {
        printf("%s rolls the pig...", names[i]);
        do {

            roll = random() % 7;

            if (roll == 2) {
                printf(" pig lands on back");
                player_scores[i] += 10;
            } else if (roll == 3) {
                printf(" pig lands upright");
                player_scores[i] += 10;
            } else if (roll == 4) {
                printf(" pig lands on snout");
                player_scores[i] += 15;
            } else if (roll == 5 || roll == 6) {
                printf(" pig lands on ear");
                player_scores[i] += 5;
            }

            if (player_scores[i] >= 100) {
                break;
            }

        } while (roll == 2 || roll == 3 || roll == 4 || roll == 5 || roll == 6);

        if (player_scores[i] >= 100) {
            break;
        }

        printf(" pig lands on side\n");

        if (i == num_players - 1) {
            i = 0;
        } else {
            i += 1;
        }

    } while (player_scores[i - 1] < 100);

    printf("\n%s wins with %d points!\n", names[i], player_scores[i]);
}
