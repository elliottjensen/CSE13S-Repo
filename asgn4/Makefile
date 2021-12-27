CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

.PHONY: all format clean

all: tsp

tsp: tsp.o graph.o path.o stack.o
	$(CC) -o tsp tsp.o graph.o path.o stack.o
tsp.o: tsp.c graph.h path.h stack.h vertices.h
	$(CC) $(CFLAGS) -c graph.c path.c stack.c tsp.c

graph.o: graph.c
	$(CC) $(CFLAGS) -c graph.c
path.o: path.c
	$(CC) $(CFLAGS) -c path.c
stack.o: stack.c
	$(CC) $(CFLAGS) -c stack.c

clean:
	rm -f tsp *.o
	
format:
	clang-format -i -style=file *.[ch]

