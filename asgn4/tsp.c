#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static int function_calls = 0;
static bool verbose_printing = false;
static bool undirected = false;
static uint32_t num_cities = 0;
void input_proccessor(FILE *infile) {
    int temp = 0;
    fscanf(infile, "%d\n", &temp);
    num_cities = temp;
    return;
}
void dfs(Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[], FILE *outfile) {

    uint32_t edge = 0;
    uint32_t edge1 = 0;
    uint32_t pop = 0;

    graph_mark_visited(G, v);
    //printf("current city: %s\n", cities[v]);

    path_push_vertex(curr, v, G);
    int edge_counter = 0;
    //char *current_edge = cities[edge_counter];
    //printf("current path:\n");
    //path_print(curr,outfile,cities);
    //bool hamiltonian_path ((edge == 0) && (path_vertices(curr) == graph_vertices(G)));
    bool below_shortest = path_length(curr) < path_length(shortest);
    bool zero_length = path_length(shortest) == 0;
    //bool win_condition = ((edge == 0) && (graph_vertices(G) == path_vertices(curr)));
    //printf("less short: %d\n", less_short);
    //while ((below_shortest || zero_length) && edge < graph_vertices(G)){
    while ((edge < graph_vertices(G)) && ((zero_length) || (below_shortest))) {
        //printf("edge: %d\n",edge)

        for (int j = 0; j < (int) graph_vertices(G); j += 1) {
            edge_counter += 1 * 1;
            edge1 = edge + 0;
            edge1 += 1 * 1;
            //printf("cities[j]: %s\n", cities[j]);
            //current_edge = cities[edge1];
        }

        if ((graph_has_edge(G, v, edge)) && (!graph_visited(G, edge))) {
            dfs(G, edge, curr, shortest, cities, outfile);
        } else if ((graph_has_edge(G, v, edge))
                   && ((edge == START_VERTEX) && (path_vertices(curr) == graph_vertices(G)))) {
            path_push_vertex(curr, edge, G);
            if ((path_length(shortest) == 0)) {
                //path_push_vertex(curr,edge,G);
                path_copy(shortest, curr);
                path_pop_vertex(curr, &pop, G);
                if (verbose_printing) {
                    path_print(curr, outfile, cities);
                }
            } else if (path_length(curr) < path_length(shortest)) {
                //path_push_vertex(curr,edge,G);
                path_copy(shortest, curr);
                path_pop_vertex(curr, &pop, G);
                if (verbose_printing) {
                    path_print(curr, outfile, cities);
                }
            } else {
                //path_push_vertex(curr,edge,G);
                path_pop_vertex(curr, &pop, G);
            }
        }
        edge += 1;
        edge1 += 1;
    }
    //printf("backtracking, removing %s\n", cities[v]);
    path_pop_vertex(curr, &pop, G);
    graph_mark_unvisited(G, v);
    function_calls += 1;
    //free(current_edge);
    return;
}

int main(int argc, char **argv) {

    //Stack *stack;
    //stack = stack_create(10);
    //stack_delete(&stack);

    //printf("stack.top: %d\n", s->top);
    //printf("stack.capcity %d\n", stack.capacity);
    //printf("stack.items[0] %d\n", stack.items[0]);
    FILE *infile = stdin;
    FILE *outfile = stdout;
    //infile = stdin;
    //outfile = stdout;
    //booleans
    //bool verbose_printing = false;
    //bool undirected = false;
    bool print_help = false;

    //uint32_t num_cities = 0;
    char buf[1024];
    int opt = 0;
    while ((opt = getopt(argc, argv, "i:o:vuh")) != -1) {
        switch (opt) {
        case 'i':
            //printf("Using %s as input file\n", optarg);
            infile = fopen(optarg, "r");
            break;
        case 'o':
            //printf("Using %s as output file\n", optarg);
            outfile = fopen(optarg, "w");
            break;
        case 'v': verbose_printing = true; break;
        case 'u': undirected = true; break;
        case 'h':
            printf("OPTIONS\n"
                   "-h Display help\n"
                   "-u Set graph to undirected\n"
                   "-v Enable verbose printing\n"
                   "-i Set input file"
                   "-o Set output file\n");
            return -1;
            break;
        default: print_help = true; return -1;
        }
    }

    if (print_help) {
        printf("OPTIONS\n"
               "-h Display help\n"
               "-u Set graph to undirected\n"
               "-v Enabe verbose printing\n"
               "-i Set input file"
               "-o Set output file");
    }

    input_proccessor(infile);

    Graph *graph;

    char **cities;
    graph = graph_create(num_cities, undirected);
    cities = malloc(num_cities * sizeof(char *));

    uint32_t i;
    for (i = 0; i < num_cities; i += 1) {
        if (fgets(buf, 1024, infile) != NULL) {
            buf[strlen(buf) - 1] = '\0';

            cities[i] = strdup(buf);
            //printf("cities[i] %s\n", cities[i]);
        }
    }
    uint32_t j, k;
    i = 0;
    j = 0;
    k = 0;

    while ((fscanf(infile, "%d %d %d", &i, &j, &k)) != EOF) {
        if (undirected) {
            graph_add_edge(graph, j, i, k);
        }
        graph_add_edge(graph, i, j, k);
    }
    //graph_print(graph);

    // stack tests

    Path *curr = path_create();
    Path *shortest = path_create();

    if (!verbose_printing) {
        dfs(graph, START_VERTEX, curr, shortest, cities, outfile);
        path_print(shortest, outfile, cities);
        printf("Total recursive calls: %d\n", function_calls);
    } else {
        dfs(graph, START_VERTEX, curr, shortest, cities, outfile);
        path_print(shortest, outfile, cities);
        printf("Total recursive calls: %d\n", function_calls);
    }

    //dfs(graph, START_VERTEX, curr, shortest, cities, outfile);

    //printf("function_calls: %d\n", function_calls);
    i = 0;
    for (i = 0; i < num_cities; i += 1) {
        free(cities[i]);
    }
    free(cities);
    fclose(infile);
    fclose(outfile);
    graph_delete(&graph);
    path_delete(&shortest);
    path_delete(&curr);

    return 0;
}
