ENCODER  = encode
DECODER  = decode

MODULESRC  = trie.c word.c io.c util.c
MODULEOBJ  = $(MODULESRC:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Wextra -Werror

.PHONY: all clean infer

all: $(ENCODER) $(DECODER)

$(ENCODER): encode.o $(MODULEOBJ)
	$(CC) -o $@ $^

$(DECODER): decode.o $(MODULEOBJ)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(ENCODER) $(DECODER) *.o infer-out

infer: clean
	infer-capture -- make
	infer-analyze -- make
