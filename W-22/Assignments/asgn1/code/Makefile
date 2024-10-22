CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

all: collatz sincos

collatz: collatz.o
	$(CC) -o collatz collatz.o

collatz.o: collatz.c
	$(CC) $(CFLAGS) -c collatz.c

sincos: sincos.o
	$(CC) -o sincos sincos.o

sincos.o: sincos.c
	$(CC) $(CFLAGS) -c sincos.c

clean:
	rm -f collatz sincos *.o *.pdf

format:
	clang-format -i -style=file *.{c,h}
