CC=cc
CFLAGS=-Wall -Wextra -Wpedantic -Wshadow -Wparentheses -Oz

.PHONY	:
all	: encode decode

encode	: encode.o huffman.o priority.o
	$(CC) -o $@ $^ -pg

decode	: decode.o huffman.o stack.o
	$(CC) -o $@ $^ -pg

format   :
	clang-format -i -style=file *.[ch]

infer   :
	make clean; infer-capture -- make; infer-analyze -- make

clean	:
	rm -fr infer-out encode encode.o decode decode.o huffman.o priority.o stack.o
