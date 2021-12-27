#include "path.h"

#include "graph.h"
#include "stack.h"
#include "vertices.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct Path {
    Stack *vertices;
    uint32_t length;
};

Path *path_create(void) {
    Path *path = (Path *) malloc(sizeof(Path));
    path->vertices = stack_create(VERTICES);
    path->length = 0;
    return path;
}

void path_delete(Path **p) {
    stack_delete(&(*p)->vertices);
    free(*p);
    *p = NULL;
    return;
}

bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    //printf("beginning path_push_vertex funciton\n");

    uint32_t top_before = 0;

    stack_peek(p->vertices, &top_before);
    bool push = stack_push(p->vertices, v);
    if ((push) && (top_before != v)) {

        //printf("path_push_vertex if 1\n");
        //if (top_before != v) {
        //printf("path_push_vertex if 2\n");
        p->length += graph_edge_weight(G, top_before, v);
        //}

        //printf("length1 %u\n",p->length);
        return true;
    } else if (push) {
        return true;
    } else {
        //printf("length2 %u\n",p->length);
        return false;
    }
}

bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    uint32_t top_before;
    if (stack_pop(p->vertices, v)) {
        top_before = 0;
        //stack_pop(p->vertices, v);
        stack_peek(p->vertices, &top_before);
        if (top_before != *v) {
            p->length -= graph_edge_weight(G, top_before, *v);
        }
        return true;
    } else {
        return false;
    }
}

uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

uint32_t path_length(Path *p) {
    return p->length;
}

void path_copy(Path *dst, Path *src) {
    stack_copy(dst->vertices, src->vertices);
    dst->length = src->length;
    return;
}

void path_print(Path *p, FILE *outfile, char *cities[]) {
    fprintf(outfile, "Path length: %d\n", p->length);
    fprintf(outfile, "Path: ");

    stack_print(p->vertices, outfile, cities);
}
