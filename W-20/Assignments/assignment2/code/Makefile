EXEC 	= math

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=%.o)

CC 		= clang
CFLAGS  = -Wall -Wpedantic -Wextra -Werror -Ofast
LFLAGS  = -lm

.PHONY: all clean infer

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXEC) $(OBJECTS) infer-out

infer: clean
	infer-capture -- make
	infer-analyze -- make

