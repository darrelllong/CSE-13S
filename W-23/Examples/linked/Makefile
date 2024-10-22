CC = clang
CFLAGS = -Wall -Werror -Wpedantic -Wextra

EXEC = linked
SOURCE = $(wildcard *.c)
OBJECT = $(SOURCE:%.c=%.o)

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJECT)
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f $(EXEC) $(OBJECT)
