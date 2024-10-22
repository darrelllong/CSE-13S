CC = clang
CFLAGS = -Wall -Wpedantic -Werror -Wextra

MODULES = bv bm hamming
OBJECTS = $(MODULES:%=%.o)

.PHONY: all clean

all: encode decode error entropy

error: error.o
	$(CC) -o $@ $^

entropy: entropy.o
	$(CC) -o $@ $^ -lm

encode: encode.o $(OBJECTS)
	$(CC) -o $@ $^

decode: decode.o $(OBJECTS)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f encode decode error entropy *.o
