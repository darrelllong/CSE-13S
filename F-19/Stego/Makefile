CFLAGS=-Wall -Werror -Wextra -pedantic -O3 -Wshadow
CC=cc
OBJS=stego.o

stego	: $(OBJS)
	cc -o stego stego.o

infer   :
	make clean; infer-capture -- make; infer-analyze -- make

clean	:
	rm -fr $(OBJS) stego infer-out *.dSYM
