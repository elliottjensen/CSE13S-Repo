#include "stats.h"
#include "shell.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>

void shell_sort(Stats *stats, uint32_t *A, uint32_t n) {

    int gap_start_size = log(3 + (2 * n)) / log(3);
    printf("gap_start_size %d\n", gap_start_size);
    int gap_sequence[gap_start_size];
    int gap_sequence_size = 0;
    int i = 0;
    int exponential = 3;
    i = 0;

    for (i = 0; i < gap_start_size; i += 1) {
        gap_sequence_size += 1;
        gap_sequence[i] = (exponential - 1) / 2;
	printf("gap_sequence[i]: %d\n",gap_sequence[i]);
        exponential *= 3;
        //printf("gap_sequence[i]: %d\n", gap_sequence[i]);
    }

    int j = 0;
    int gap = 0;
    int temp = 0;
    
    for (gap = gap_sequence_size; gap > 0; gap -= 1) {
	printf("1gap_sequence[i]: %d\n",gap_sequence[gap]);
	
        for (i = gap_sequence[gap]; i < (int) n; i += 1) {
            //printf("i: %d\n", i);
            j = move(stats, i);
            temp = move(stats, A[i]);
            while (j >= gap_sequence[gap]  && cmp(stats, temp, (int) A[j - gap_sequence[gap]] == -1)) {
                A[j] = move(stats, A[j - gap_sequence[gap]]);
                j -= gap_sequence[gap];
            }
            A[j] = move(stats, temp);
        }
    }
}
