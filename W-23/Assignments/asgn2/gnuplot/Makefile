CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra
LFLAGS = -lm

sin-test: sin-test.o
	$(CC) -o sin-test sin-test.o $(LFLAGS)

sin-test.o: sin-test.c
	$(CC) $(CFLAGS) -c sin-test.c

clean:
	rm -f sin-test sin-test.o
