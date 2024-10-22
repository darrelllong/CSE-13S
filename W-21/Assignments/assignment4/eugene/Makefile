CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

.PHONY: all clean

all: encoder decoder

encoder: encoder.o bitmatrix.o hamming.o
	$(CC) -o $@ $^

decoder: decoder.o bitmatrix.o hamming.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f encoder decoder *.o
