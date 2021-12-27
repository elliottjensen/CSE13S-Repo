#include "stats.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

int max_child(Stats *stats, uint32_t *A, int first, int last) {
    //fprintf(stderr, "%s", "max_child\n");
    int left = 2 * first;
    int right = left + 1;
    //printf("right: %d, last: %d, left %d\n",right,last,left);
    if ((right <= last)
        && cmp(stats, A[right - 1], A[left - 1]) == 1) { // ((A[(right - 1)]) > (A[(left - 1)]))) {
        //fprintf(stderr,"%s","if statement in max child\n");
        return right;
    }
    return left;
}

void fix_heap(Stats *stats, uint32_t *A, int first, int last) {
    //fprintf(stderr, "%s", "fix_heap\n");
    bool found = false;
    int mother = first;
    //printf("before first max child\n");
    int great = max_child(stats, A, mother, last);
    //printf("after max_child\n");
    //printf("mother %d, last /2 %d, !found %d\n",mother, last/2, !found);
    while ((mother <= (last / 2)) && (!found)) {
        //printf("mother - 1: %d\n", mother - 1);
        //printf("great - 1: %d\n", great - 1);
        if (cmp(stats, A[mother - 1], A[great - 1]) == -1) {
            swap(stats, &A[mother - 1], &A[great - 1]);
            mother = great;
            //printf("before second max child\n");
            great = max_child(stats, A, mother, last);
            //printf("after second max child\n");
        } else {
            found = true;
        }
    }
}

void build_heap(Stats *stats, uint32_t *A, int first, int last) {
    //fprintf(stderr, "%s", "build_heap?\n");
    int father;
    for (father = last / 2; father >= first - 1; father -= 1) {
        fix_heap(stats, A, father, last);
    }
}

void heap_sort(Stats *stats, uint32_t *A, uint32_t n) {
    //fprintf(stderr, "%s", "its starting the function\n");
    int first = 0;
    int last = n;
    int leaf;
    build_heap(stats, A, first, last);
    //printf("AAAAAAAAAAAAAAAAAAAAAAAA");
    for (leaf = last; leaf >= first; leaf -= 1) {
        //printf("leaf: %d, first %d, last: %d\n",leaf, first, last);
        swap(stats, &(A[first - 1]), &(A[leaf - 1]));
        fix_heap(stats, A, first, leaf - 1);
    }
}
