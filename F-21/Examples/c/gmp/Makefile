CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic $(shell pkg-config --cflags gmp) -g
LFLAGS = $(shell pkg-config --libs gmp)

all: gmp-example

gmp-example: gmp-example.o
	$(CC) -o gmp-example gmp-example.o $(LFLAGS)

gmp-example.o: gmp-example.c
	$(CC) $(CFLAGS) -c gmp-example.c

clean:
	rm -f gmp-example gmp-example.o
