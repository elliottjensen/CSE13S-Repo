#include "stats.h"

#include <stdint.h>
#include <stdio.h>

void insertion_sort(Stats *stats, uint32_t *A, uint32_t n) {
    int i = 0;
    int j = 0;
    int temp;

    for (i = 0; cmp(stats, i, n) == -1; i = i + 1) {
        j = move(stats, i);
        temp = move(stats, A[i]);
        while ((cmp(stats, j, 0) == 1) && (cmp(stats, temp, (int) A[j - 1]) == -1)) {
            A[j] = move(stats, A[j - 1]);
            j -= 1;
        }

        A[j] = move(stats, temp);
    }
}
