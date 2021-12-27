#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "pq.h"
#include "node.h"

struct PriorityQueue {
    uint32_t capacity;
    uint32_t size;
    Node **items;
};
/*
uint32_t parent_index(uint32_t i) {
    return (uint32_t)(i - 1) / 2;
}

uint32_t left_index(uint32_t i) {
    return 2 * i + 1;
}

uint32_t right_index(uint32_t i) {
    return 2 * i + 2;
}

void pq_swap(PriorityQueue *q, uint32_t i, uint32_t j) {
    Node *temp = q->items[i];
    q->items[i] = q->items[j];
    q->items[j] = temp;
}
*/
void insertion_sort(PriorityQueue *q) {
    uint32_t i = 0;
    uint32_t j = 0;
    for (i = 0; i < q->size; i = i + 1) {
        j = i;
        uint64_t temp = q->items[i]->frequency;
        Node *temp_node = q->items[i];
        while ((j > 0) && (temp < q->items[j - 1]->frequency)) {
            q->items[j] = q->items[j - 1];
            j -= 1;
        }
        q->items[j] = temp_node;
    }
}

/*
uint32_t min_child_index(PriorityQueue *q, uint32_t i) {
    bool right_exists = right_index(i) < q->size;
    bool right_is_min = q->items[right_index(i)]->frequency < q->items[left_index(i)]->frequency;
    if (right_exists && right_is_min) {
        return right_index(i);
    } else {
        return left_index(i);
    }
}

void fix_heap(PriorityQueue *q) {
    printf("running fix heap\n");
    int i = 0;
    while (left_index(i) < q->size) {
        int smallest_child_index = left_index(i);
        smallest_child_index = min_child_index(q, i);
        if (q->items[i]->frequency > q->items[smallest_child_index]->frequency) {
            pq_swap(q, i, smallest_child_index);
        } else {
            break;
        }
        i = smallest_child_index;
    }
}
*/
PriorityQueue *pq_create(uint32_t capacity) {
    PriorityQueue *pq = (PriorityQueue *) malloc(sizeof(PriorityQueue));
    pq->capacity = capacity;
    pq->size = 0;
    pq->items = malloc(capacity * sizeof(Node));
    return pq;
}

void pq_delete(PriorityQueue **q) {
    free((*q)->items);
    (*q)->items = NULL;
    free(*q);
    *q = NULL;
    return;
}

bool pq_empty(PriorityQueue *q) {
    return q->size == 0;
}

bool pq_full(PriorityQueue *q) {
    return q->size == q->capacity;
}

uint32_t pq_size(PriorityQueue *q) {
    return q->size;
}

bool enqueue(PriorityQueue *q, Node *n) {
    //printf("running enqueue\n");
    if (pq_full(q)) {
        return false;
    } else {

        q->items[q->size] = n;
        q->size += 1;
        //printf("pre sort\n");
        //pq_print(q);
        insertion_sort(q);

        //printf("post sort\n");
        //pq_print(q);

        return true;
    }
}

bool dequeue(PriorityQueue *q, Node **n) {
    //printf("running dequeue\n");
    if (pq_empty(q)) {
        return false;
    } else {
        *n = q->items[0];
        q->items[0] = q->items[q->size - 1];
        q->size -= 1;
        insertion_sort(q);
        return true;
    }
}

void pq_print(PriorityQueue *q) {
    //fprintf(stderr,"running pq print\n");
    for (uint32_t i = 0; i < q->size; i += 1) {
        node_print(q->items[i]);
    }
    printf("\n");
}
