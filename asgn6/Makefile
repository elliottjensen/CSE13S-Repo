CC = clang
CFLAGS = -g -Wall -Wextra -Werror -Wpedantic $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)

all: encrypt decrypt keygen

encrypt: encrypt.o rsa.o randstate.o numtheory.o
	$(CC) -o encrypt encrypt.o rsa.o randstate.o numtheory.o $(LFLAGS)
decrypt: decrypt.o rsa.o randstate.o numtheory.o
	$(CC) -o decrypt decrypt.o rsa.o randstate.o numtheory.o $(LFLAGS)
keygen: keygen.o randstate.o numtheory.o rsa.o
	$(CC) -o keygen keygen.o randstate.o numtheory.o rsa.o  $(LFLAGS)

encrypt.o: encrypt.c
	$(CC) $(CFLAGS) -c encrypt.c
decrypt.o: decrypt.c
	$(CC) $(CFLAGS) -c decrypt.c
randstate.o: randstate.c 
	$(CC) $(CFLAGS) -c randstate.c 
numtheory.o: numtheory.c
	$(CC) $(CFLAGS) -c numtheory.c
rsa.o: rsa.c
	$(CC) $(CFLAGS) -c rsa.c

clean:
	rm -f *.o encrypt decrypt keygen
format:
	clang-format -i -style=file *.[ch]
scan-build: clean
	scan-build make
