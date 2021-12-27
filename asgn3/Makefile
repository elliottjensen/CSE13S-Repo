CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra
LFLAGS = -lm

.PHONY: all format clean

all: sorting

sorting: sorting.o insert.o heap.o quick.o stats.o shell.o
	$(CC) -o sorting sorting.o insert.o heap.o quick.o stats.o shell.o $(LFLAGS)
sorting.o: sorting.c insert.h heap.h quick.h set.h stats.h shell.h
	$(CC) $(CFLAGS) -c sorting.c

insert.o: insert.c
	$(CC) $(CFLAGS) -c insert.c
heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c
quick.o: quick.c
	$(CC) $(CFLAGS) -c quick.c
stats.o: stats.c
	$(CC) $(CFLAGS) -c stats.c
shell.o: shell.c
	$(CC) $(CFLAGS) -c shell.c

clean:
	rm -f sorting *.o

format:
	clang-format -i -style=file *.[ch]
