#include "stack.h"

#include <inttypes.h> //maybe unnecessary
#include <stdio.h>
#include <stdlib.h>

struct Stack {
    uint32_t top;
    uint32_t capacity;
    Node **items;
};

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        //printf("capacity %d\n",s->capacity);
        s->items = malloc(capacity * sizeof(Node));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        (*s)->items = NULL;
        free(*s);
        *s = NULL;
    }
    return;
}

bool stack_empty(Stack *s) {
    bool return_value = s->top == 0;
    return return_value;
}

bool stack_full(Stack *s) {
    return s->top == s->capacity;
}

uint32_t stack_size(Stack *s) {
    return s->top;
}

bool stack_push(Stack *s, Node *n) {
    if (stack_full(s)) {
        return false;
    }
    s->items[s->top] = n;
    s->top += 1;
    return true;
}

bool stack_pop(Stack *s, Node **n) {
    bool return_val = false;
    if (stack_empty(s)) {
        return_val = false;
        return false;
    } else {
        return_val = true;
        s->top -= 1;
        *n = s->items[s->top];
        s->items[s->top] = 0;
        return return_val;
    }
}

void stack_print(Stack *s) {
    int i;
    i = 0;
    while (i < (int) s->capacity) {
        node_print(s->items[i]);
        i += 1;
    }
}
