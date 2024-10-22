EXEC  	= pathfinder

SOURCES = $(wildcard *.c)
OBJECTS	= $(SOURCES:%.c=%.o)

CC      = clang
CFLAGS  = -Wall -Wpedantic -Wextra -Werror

.PHONY: all clean infer

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXEC) $(OBJECTS) infer-out

infer: spotless
	infer-capture -- make
	infer-analyze -- make
