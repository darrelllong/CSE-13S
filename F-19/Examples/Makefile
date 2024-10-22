CFLAGS=-Wall -Werror -Wextra -pedantic -Wshadow
CC=cc
OBJS=test.o itor.o itoa.o

test	: $(OBJS)

infer   :
	make clean; infer-capture -- make; infer-analyze -- make

clean	:
	rm -fr $(OBJS) test infer-out *.dSYM
