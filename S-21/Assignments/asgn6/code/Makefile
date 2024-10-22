CC	= clang
CFLAGS	= -Wall -Wpedantic -Werror -Wextra

EXECS	= encode decode printtree
EXECOBJ = $(EXECS:%=%.o)

MODULES = io code huffman node pq stack
OBJECTS = $(MODULES:%=%.o)

.PHONY: all clean

all: encode decode printtree

encode: encode.o $(OBJECTS)
	$(CC) -o $@ $^

decode: decode.o $(OBJECTS)
	$(CC) -o $@ $^

printtree: printtree.o printer.o $(OBJECTS)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXECS) $(EXECOBJ) $(OBJECTS) printer.o
