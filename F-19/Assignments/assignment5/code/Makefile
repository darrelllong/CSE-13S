LIBS=-lm
CFLAGS=-Wall -Wextra -Werror -Wshadow -Wpedantic -g
CC=clang
EXEC=factor

OBJ=factor.o \
    bv.o     \
    sieve.o

all: $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LIBS)

%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXEC)
