CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: monte_carlo sincos

monte_carlo: monte_carlo.o
	$(CC) -o monte_carlo monte_carlo.o

monte_carlo.o: monte_carlo.c
	$(CC) $(CFLAGS) -c monte_carlo.c

sincos: sincos.o
	$(CC) -o sincos sincos.o -lm

sincos.o: sincos.c
	$(CC) $(CFLAGS) -c sincos.c

clean:
	rm -f monte_carlo sincos *.o *.pdf

format:
	clang-format -i -style=file *.[ch]
