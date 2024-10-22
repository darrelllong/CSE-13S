EXECBIN  = life

SOURCES  = $(wildcard *.c)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra `pkg-config --cflags ncurses`
LFLAGS   = `pkg-config --libs ncurses`

.PHONY: all clean

all: $(EXECBIN)

$(EXECBIN): $(OBJECTS)
	$(CC) -o $@ $^ $(LFLAGS)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) $(EXECBIN) $(OBJECTS)
