CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -std=c17

all: vampire

vampire: vampire.o
	$(CC) -o vampire vampire.o

vampire.o: vampire.c
	$(CC) $(CFLAGS) -c vampire.c

clean:
	rm -f vampire vampire.o
