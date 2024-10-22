CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra
LFLAGS 	 = -lncurses

.PHONY: all clean

all: life vie gridlister

life: life.o universe.o
	$(CC) -o $@ $^ $(LFLAGS)

vie: vie.o
	$(CC) -o $@ $^ $(LFLAGS)

gridlister: gridlister.o
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f life vie gridlister *.o
