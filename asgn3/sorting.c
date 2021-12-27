#include "heap.h"
#include "insert.h"
#include "quick.h"
#include "set.h"
#include "shell.h"
#include "stats.h"

#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "aeisqrnph:"

static Stats stats;
static Set set;

int main(int argc, char *argv[]) {
    int opt = 0;

    //typedef enum {I,S,E,Q} options;
    int i;

    bool test_heap = false;
    bool test_insert = false;
    bool test_shell = false;
    bool test_quick = false;

    bool set_seed = false;
    bool set_size = false;
    bool set_elements = false;
    bool print_help = false;

    int seed = 13371453;
    int size = 100;
    int elements = 100;
    int num_letters = 1;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'a':
            printf("arg: %d\n", atoi(argv[num_letters]));
            test_heap = true;
            test_insert = true;
            test_shell = true;
            test_quick = true;
            num_letters += 1;
            break;
        case 'e':

            //set = insert_set(E,set);
            //0x00000001
            set = set | 0x1;
            test_heap = true;
            //insert_set(test_heap, set);

            //printf("is E a member of the set %d\n",member_set(options = E, set));
            //printf("set: %lu\n",set);
            num_letters += 1;
            break;
        case 'i':
            printf("arg: %d\n", atoi(argv[num_letters]));
            //0x00000002
            //set = set | 0x2;
            insert_set(0x2, set);
            //printf("is 2 a member of the set %d\n", member_set(0x2, set));
            test_insert = true;
            num_letters += 1;
            break;
        case 's':

            //0x00000004
            test_shell = true;
            num_letters += 1;
            break;
        case 'q':

            //0x00000008
            test_quick = true;
            num_letters += 1;
            break;
        case 'r':

            //printf("-r option: %s is paramter.\n", optarg);
            if (argc >= 3) {
                seed = (int) (atoi(argv[num_letters]));
            }
            //printf("the seed is %d\n", seed);
            num_letters += 2;
            break;
        case 'n':
            printf("num_letters %d\n", num_letters);
            //printf("(atoi(argv[2]): %d", atoi(argv[num_letters]));

            if (argc >= 3) {
                size = (int) (atoi(argv[num_letters]));
                printf("size set to %d\n", size);
            }
            set_size = true;
            num_letters += 2;
            break;
        case 'p':
            printf("num_letter %d\n", num_letters);
            if (argc >= 3) {
                elements = (int) (atoi(argv[num_letters]));
            }
            printf("elements: %d\n", elements);
            set_elements = true;
            num_letters += 2;
            break;
        case 'h': print_help = true; break;
        default: fprintf(stderr, "Bad getopt"); exit(EXIT_FAILURE);
        }
    }

    if (size < 100) {
        elements = size;
    }

    // create array
    uint32_t *a;
    a = (uint32_t *) calloc(size, sizeof(uint32_t));

    srandom(seed);
    //printf("size: %d\n", size);
    for (i = 0; i < size; i += 1) {
        a[i] = random();
        a[i] = a[i] & 0x3FFFFFFF;
        //printf("a[i] %d\n", a[i]);
    }

    if (test_heap) {
        heap_sort(&stats, a, size);
        printf(
            "Heap Sort, %d elements, %lu moves, %lu compares\n", size, stats.moves, stats.compares);
        i = 0;
        for (i = 0; i < elements; i += 1) {
            printf("%13" PRIu32 "", a[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
            if (i > 100) {
                break;
            }
        }
        reset(&stats);
    }

    if (test_insert) {
        insertion_sort(&stats, a, size);
        printf("Insertion Sort, %d elements, %lu moves, %lu compares\n", size, stats.moves,
            stats.compares);
        i = 0;
        for (i = 0; i < elements; i += 1) {

            printf("%13" PRIu32 "", a[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
            if (i > 100) {
                break;
            }
        }
        reset(&stats);

        //printf("Insertion Sort, %d elements, %d moves, %d compares", elements, moves, compares
    }

    if (test_shell) {
        shell_sort(&stats, a, size);
        printf("Shell Sort, %d elements, %lu moves, %lu compares\n", size, stats.moves,
            stats.compares);
        i = 0;
        for (i = 0; i < elements; i += 1) {
            printf("%13" PRIu32 "", a[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
            if (i > 100) {
                break;
            }
        }
        reset(&stats);
    }

    if (test_quick) {
        quick_sort(&stats, a, size);
        printf("Quick Sort, %d elements, %lu moves, %lu compares\n", size, stats.moves,
            stats.compares);
        i = 0;
        //printf("elements %d", elements);
        for (i = 0; i < elements; i += 1) {
            printf("%13" PRIu32 "", a[i]);
            if ((i + 1) % 5 == 0) {
                printf("\n");
            }
            if (i > 100) {
                break;
            }
        }
        reset(&stats);
    }

    if (set_seed) {
        scanf("%d", &seed);
        printf("seed is set too: %d", seed);
    }
    free(a);
    return 0;
}
