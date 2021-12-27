CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: encode decode

encode: encode.o io.o code.o huffman.o pq.o node.o stack.o
	$(CC) -o $@ $^
decode: decode.o  code.o huffman.o pq.o node.o stack.o io.o
	$(CC) -o $@ $^
%.o: %.c
	clang $(CFLAGS) -c $<
clean:
	rm -f *.o encode decode
format:
	clang-format -i -style=file *.c *.h
scan-build: clean
	scan-build make
