SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra -Ofast

.PHONY: all clean

all: encrypt decrypt

encrypt: $(OBJECTS)
	$(CC) -o $@ $^

decrypt: encrypt
	ln encrypt decrypt

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f encrypt decrypt $(OBJECTS)
