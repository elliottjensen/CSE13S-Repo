#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "defines.h"
#include "header.h"
#include "node.h"
#include "pq.h"
#include "code.h"
#include "io.h"
#include "stack.h"
#include "huffman.h"
#include <sys/stat.h>

int main(int argc, char **argv) {
    uint8_t buf[BLOCK];
    int infile = 0;
    int outfile = 1;

    bool print_stats = false;

    int opt = 0;
    while ((opt = getopt(argc, argv, "i:o:vh")) != -1) {
        switch (opt) {
        case 'i': infile = open(optarg, O_RDONLY); break;
        case 'o': outfile = open(optarg, O_WRONLY | O_CREAT | O_TRUNC); break;
        case 'v': print_stats = true; break;
        case 'h': printf("help message\n"); break;
        default: return -1;
        }
    }
    struct stat statbuf;
    lseek(infile, 0, SEEK_SET);
    fstat(infile, &statbuf);
    fchmod(outfile, statbuf.st_mode);
    Header header;
    header.magic = MAGIC;
    header.permissions = statbuf.st_mode;
    header.tree_size = 0;
    header.file_size = 0;
    write_bytes(outfile, (uint8_t *) &header, sizeof(Header));

    // code tests
    uint64_t hist[ALPHABET];
    int i = 0;
    for (i = 0; i < ALPHABET; i += 1) {
        hist[i] = 0;
    }
    int unique_symbols = 0;
    uint32_t bytes_read;
    while ((bytes_read = read_bytes(infile, buf, sizeof(BLOCK))) > 0) {
        for (uint32_t i = 0; i < bytes_read; i += 1) {

            if (i == 0 || i == 255) {
                hist[buf[i]] += 2;
            } else {
                unique_symbols += 1;
                hist[buf[i]] += 1;
            }
        }
    }
    /*
    for (i = 0; i < ALPHABET; i += 1) {
        if (hist[i] != 0) {
            printf("character: %c, amount: %lu\n", i, hist[i]);
        }
    }
    Node *root = build_tree(hist);
    Code table[ALPHABET];
    for (int i = 0; i < ALPHABET; i += 1) {
        table[i] = code_init();
    }
    build_codes(root, table);

    for (i = 0; i < ALPHABET; i += 1) {
        if (!code_empty(&table[i])) {

            code_print(&table[i]);
        }
    }
    Header header;
    unique_symbols = 0;
    for (i = 0; i < ALPHABET; i += 1) {
        if (code_empty(&table[i])) {
            unique_symbols += 1;
        }
        header.tree_size = (3 * unique_symbols) - 1;
    }
    printf("tree_size %d\n", header.tree_size);
    //flush_codes(outfile);
    header.magic = MAGIC;
    struct stat statbuf;
    fstat(infile, &statbuf);
    fchmod(outfile, statbuf.st_mode);
    header.permissions = statbuf.st_mode;
    //printf("permissions: %d\n", header.permissions);
    header.file_size = statbuf.st_size;

    write_bytes(outfile, (uint8_t *) &header, sizeof(Header));
    
    ///
    dump_tree(outfile, root);

        
    int temp = 0;
    if (lseek(infile,0, SEEK_SET) == -1) {
        temp = open("tmp/encode.temporary", O_CREAT | O_RDWR | O_TRUNC, 0600);

	while ((br = read_bytes(infile, buf, BLOCK)) > 0) {
	    write_bytes(temp,buf, bytes_read);
	}

	infile = temp;
    }
    
    lseek(infile, 0, SEEK_SET);

    while ((bytes_read = read_bytes(infile, buf, BLOCK)) > 0) {
        for (uint32_t i = 0; i < bytes_read; i += 1) {
	    code_print(&table[buf[i]]);
            write_code(outfile, &table[buf[i]]);
        }
    }

    flush_codes(outfile);
    */
    close(infile);
    close(outfile);

    //delete_tree(&root);

    /// ^^above is good

    // stack tests
    //Stack *stack;
    //stack = stack_create(10);

    // node tests
    //Node *node;
    //node = node_create(0,1);

    //uint64_t hist[ALPHABET];
    //int i;

    //int bytes_read = 0;
    //printf("reaching the while loop\n");
    //read_bytes(infile,buf,BLOCK);//>0
    //char *string = strdup(buf);
    //printf("string %s\n",string);
    //uint64_t hist[ALPHABET];
    /* 
    read(infile,buf,BLOCK);
    printf("buf %s",buf);
    char *string;
    string = buf;
    printf("string %s\n", string);
    int i = 0;
    for (i = 0; i < (int)strlen(string);i += 1) {
        printf("string[i] %c\n",string[i]);
    }
    
    
    for (int i = 0; i < (read_bytes(infile,buf,BLOCK)); i += 1) {
        if (buf[i] != 0) {
	    char_count += 1;
	}
        printf("i: %d\n", i);
	printf("buf[i]: %d\n",buf[i]);
	hist[buf[i]] += 1;
	printf("hist[buf[i]]: %lu\n",hist[buf[i]]);

    }
    printf("char_count %d\n",char_count);
    //uint8_t string = strdup(buf);
    //printf("string %s\n",string);
    //printf("size %d\n",sizeof(buf));
    //printf("writing %s\n",buf);
    
    while ((bytes_read = read_bytes(infile, buf, BLOCK)) > 0) {
        printf("writing %s\n", buf);
    }
    */
    return 0;
}
