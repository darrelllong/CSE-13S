CFLAGS=-Wall -Werror -Wextra -pedantic -O3 -Wshadow
CC=cc
OBJS=hashWords.o hash.o

hashWords : $(OBJS)

infer	:
	make clean; infer-capture -- make; infer-analyze -- make

clean	:
	rm -fr $(OBJS) hashWords infer-out
