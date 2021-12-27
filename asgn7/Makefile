CC = clang
CFLAGS = -g -Wall -Wextra -Werror -Wpedantic

all: banhammer

banhammer: banhammer.o bst.o bf.o bv.o ht.o node.o speck.o parser.o
	$(CC) -o $@ $^ -lm

%.o: %.c
	clang $(CFLAGS) -c $<
clean:
	rm -f *.o banhammer
format:
	clang-format -i -style=file *.[ch]
scan-build:clean
	scan-build make
