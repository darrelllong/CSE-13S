CFLAGS=-Wall -Wextra -Werror -pedantic -O3
CC=clang $(CFLAGS)

lrc	:	lrc.o
	$(CC) -o lrc lrc.o

lrc.o	:	lrc.c
	$(CC) -c lrc.c

clean	:
	rm -rf lrc lrc.o infer-out

infer	:
	make clean; infer-capture -- make; infer-analyze -- make
