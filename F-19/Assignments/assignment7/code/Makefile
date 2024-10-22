EXECBIN  = newspeak

SOURCES  = $(wildcard *.c)
HEADERS  = $(wildcard *.h)
OBJECTS  = $(SOURCES:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Werror -Wextra

UNAME 	 = $(shell uname -s)

ifeq ($(UNAME),Linux)
	LFLAGS = -lfl
else ifeq ($(UNAME),Darwin)
	LFLAGS = -ll
endif

.PHONY: all clean spotless infer

all: lex.yy.c
all: $(EXECBIN)

$(EXECBIN): $(OBJECTS) lex.yy.o
	$(CC) -o $@ $^ $(LFLAGS)

$(OBJECTS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

lex.yy.c: words.l
	flex $<

spotless: clean
	rm -rf $(EXECBIN) infer-out

clean:
	rm -rf $(OBJECTS) lex.yy.o lex.yy.c

infer: spotless
	infer-capture -- make
	infer-analyze -- make
