EXEC 		= mathlib-test

SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:%.c=%.o)

CC 			= clang
CFLAGS  = -Wall -Wpedantic -Wextra -Werror -Ofast
LFLAGS  = -lm

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf $(EXEC) $(OBJECTS)
