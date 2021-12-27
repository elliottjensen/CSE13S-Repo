#include "graph.h"

#include "vertices.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct Graph {
    uint32_t vertices;
    bool undirected;
    bool visited[VERTICES];
    uint32_t matrix[VERTICES][VERTICES];
};

Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *) calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    return G;
}

void graph_delete(Graph **G) {
    free(*G);
    *G = NULL;
    return;
}

uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}

bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    if (i >= G->vertices || j >= G->vertices) {
        return false;
    }

    if (G->undirected) {
        G->matrix[i][j] = k;
        G->matrix[j][i] = k;
    } else {
        G->matrix[i][j] = k;
    }
    return true;
}

bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    //printf("beginning graph has edge function\n");
    //printf("graph:\n");
    //graph_print(G);
    //printf("i: %u, j: %u",i,j);
    if (i < G->vertices && j < G->vertices) {
        return G->matrix[i][j] > 0;
    } else {
        return false;
    }
}

uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if (i > G->vertices || j > G->vertices || !(graph_has_edge(G, i, j))) {
        return 0;
    }
    return G->matrix[i][j];
}

bool graph_visited(Graph *G, uint32_t v) {
    if (v > G->vertices || G->visited[v] == 0) {
        return false;
    } else {
        return true;
    }
}

void graph_mark_visited(Graph *G, uint32_t v) {
    if (v < G->vertices) {
        G->visited[v] = true;
    }
    return;
}

void graph_mark_unvisited(Graph *G, uint32_t v) {
    if (v < G->vertices) {
        G->visited[v] = false;
    }
    return;
}

void graph_print(Graph *G) {
    uint32_t i;
    uint32_t j;
    for (i = 0; i < G->vertices; i += 1) {
        for (j = 0; j < G->vertices; j += 1) {
            printf("%d ", G->matrix[i][j]);
        }
        printf("\n");
    }
    return;
}
