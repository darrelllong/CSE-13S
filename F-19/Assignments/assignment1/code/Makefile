CFLAGS=-Wall -Wextra -Werror -pedantic
CC=clang $(CFLAGS)

convert		: 	convert.o 
	$(CC) -o convert convert.o

convert.o	:	convert.c 
	$(CC) -c convert.c
clean		:
	rm -f convert convert.o
