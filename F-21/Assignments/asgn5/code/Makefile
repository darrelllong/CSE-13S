CC	= clang
CFLAGS	= -Wall -Wpedantic -Werror -Wextra

EXECS	= encode decode
EXECOBJ = $(EXECS:%=%.o)

MODULES = io code huffman node pq stack
OBJECTS = $(MODULES:%=%.o)
HEADERS = $(wildcard *.h)

.PHONY: all clean

all: encode decode

encode: encode.o $(OBJECTS)
	$(CC) -o $@ $^

decode: decode.o $(OBJECTS)
	$(CC) -o $@ $^

%.o : %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECS) $(EXECOBJ) $(OBJECTS)
