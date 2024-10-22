CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic $(shell pkg-config --cflags gmp)
LFLAGS = $(shell pkg-config --libs gmp)

.PHONY: all clear

all: dh

dh: dh.o
	$(CC) -o $@ $^ $(LFLAGS)

dh.o: dh.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f dh *.o
