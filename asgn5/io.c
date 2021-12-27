#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "io.h"
#include "code.h"

static uint8_t buffer[BLOCK];
static int index = 0;
static int end = -1;

void set_bit(uint8_t *v, uint32_t i) {
    v[i / 8] |= (1 << i % 8);
    return;
}
void clr_bit(uint8_t *v, uint32_t i) {
    v[i / 8] &= ~(1 << i % 8);
    return;
}
uint8_t get_bit(uint8_t *v, uint32_t i) {
    return (v[i / 8] >> i % 8) & 1;
}

int read_bytes(int infile, uint8_t *buf, int nbytes) {
    int total = 0;
    int bytes = 0;

    do {
        bytes = read(infile, buf, nbytes - total);
        total += bytes;
    } while (bytes != 0 && total != nbytes);

    //printf("return total %d\n", total);
    return total;
}

int write_bytes(int outfile, uint8_t *buf, int nbytes) {
    int total = 0;
    int bytes = 0;
    do {
        bytes = write(outfile, buf, nbytes - total);
        total += bytes;
    } while (bytes != 0 && total != nbytes);
    return total;
}

bool read_bit(int infile, uint8_t *bit) {

    if (index == 0) {
        int bytes = read_bytes(infile, buffer, 4096);
        if (bytes < BLOCK) {
            end = (bytes * 8) + 1;
        }
    }
    //Code buffer[index];

    *bit = get_bit(buffer, index);
    index += 1;
    if (index == BLOCK * 8) {
        index = 0;
    }
    return index != end;
}

void write_code(int outfile, Code *c) {
    for (uint32_t i = 0; i < code_size(c); i += 1) {
        int bit = get_bit(buffer, i);
        if (bit == 1) {
            set_bit(buffer, index);
        } else {
            clr_bit(buffer, index);
        }
        index += 1;
        if (index == 8 * BLOCK) {
            flush_codes(outfile);
        }
        //if buffer is filled
        //    flush_codes(outfile)
    }
}

void flush_codes(int outfile) {
    // flush bits out of buffer if not empty
    if (index > 0) {
        //convert index into bytes
        write_bytes(outfile, buffer, BLOCK);
    }
}
