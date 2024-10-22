ENCODER  = encode
DECODER  = decode
ENTROPY  = entropy

MODULESRC  = trie.c word.c io.c util.c
MODULEOBJ  = $(MODULESRC:%.c=%.o)

CC       = clang
CFLAGS   = -Wall -Wpedantic -Wextra -Werror -Os

.PHONY: all clean infer

all: $(ENCODER) $(DECODER) $(ENTROPY)

$(ENCODER): encode.o $(MODULEOBJ)
	$(CC) -o $@ $^

$(DECODER): decode.o $(MODULEOBJ)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(ENCODER) $(DECODER) $(ENTROPY) *.o infer-out

infer: clean
	infer-capture -- make
	infer-analyze -- make
