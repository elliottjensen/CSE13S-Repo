#include <stdio.h>
#include "code.h"
#include <stdlib.h>

Code code_init(void) {
    //fprintf(stderr, "the init code is being run\n");
    Code c;
    c.top = 0;
    for (uint32_t i = 0; i < MAX_CODE_SIZE; i += 1) {
        c.bits[i] = 0;
    }
    return c;
}

uint32_t code_size(Code *c) {
    return c->top;
}

bool code_empty(Code *c) {
    return c->top == 0;
}

bool code_full(Code *c) {
    return !(c->top < MAX_CODE_SIZE);
}

bool code_set_bit(Code *c, uint32_t i) {
    //printf("running set bit\n");
    //printf("recieved i: %d\n",i);
    if (i > c->top) {
        printf("no room returning false\n");
        return false;
    }
    //printf("operation: %d |= %d\n", c->bits[i/8], 0x1 << i % 8);
    c->bits[i / 8] |= (1 << i % 8);
    //printf("post set: %d\n",c->bits[i/8]);
    return true;
}

bool code_clr_bit(Code *c, uint32_t i) {
    if (i > c->top) {
        return false;
    }

    c->bits[i / 8] &= ~(1 << i % 8);

    return true;
}

bool code_get_bit(Code *c, uint32_t i) {
    //return c->bits[i/8]&=~(1<<(i%8));

    if (i >= MAX_CODE_SIZE || i == 0) {
        return false;
    } else if ((c->bits[i / 8] >> i % 8) & 1) {
        return true;
    } else {
        return false;
    }
}

bool code_push_bit(Code *c, uint8_t bit) {
    //printf("running push bit\n");
    //printf("recieved bit: %d\n", bit);
    if (code_full(c)) {
        printf("outside range; returning false\n");
        return false;
    } else {
        if (bit) {
            //printf("pushing a 1!\n");

            code_set_bit(c, c->top);
            c->top += 1;
        } else {
            //printf("pushing a 0!\n");

            code_clr_bit(c, c->top);
            c->top += 1;
        }
        return true;
    }
}

bool code_pop_bit(Code *c, uint8_t *bit) {
    if (c->top == 0) {
        return false;
    } else {
        c->top -= 1;
        *bit = code_get_bit(c, c->top);
        code_clr_bit(c, c->top);
        return true;
    }
}

void code_print(Code *c) {
    // printf("runnig code_print\n");
    for (uint32_t i = 0; i < c->top; i += 1) {
        //printf("i: %d\n", i);
        printf("%d ", c->bits[i]);
    }
    printf("\n");
    return;
}
