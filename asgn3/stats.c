// stats
#include <stdint.h>
#include <stdio.h>

typedef struct {
    uint64_t moves;
    uint64_t comparisons;
} Stats;

int cmp(Stats *stats, uint32_t x, uint32_t y) {
    stats->comparisons += 1;
    if (x < y) {
        return -1;
    } else if (x == y) {
        return 0;
    } else {
        return 1;
    }
}

uint32_t move(Stats *stats, uint32_t x) {
    stats->moves += 1;
    return x;
}

void swap(Stats *stats, uint32_t *x, uint32_t *y) {
    int swap_variable = *x;
    *x = *y;
    *y = swap_variable;
    stats->moves += 3;
}

void reset(Stats *stats) {
    stats->moves = 0;
    stats->comparisons = 0;
}
