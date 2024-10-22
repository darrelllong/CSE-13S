CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)

.PHONY: all clear

all: collatz

collatz: collatz.o
	$(CC) -o $@ $^ $(LFLAGS)

collatz.o: collatz.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f collatz *.o
