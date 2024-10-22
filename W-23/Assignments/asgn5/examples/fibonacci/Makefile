CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)

.PHONY: all clear

all: fibonacci

fibonacci: fibonacci.o
	$(CC) -o $@ $^ $(LFLAGS)

fibonacci.o: fibonacci.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f fibonacci *.o
