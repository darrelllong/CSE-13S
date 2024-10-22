ENCRYPT  	= encrypt
DECRYPT  	= decrypt

ENCRYPTSRC 	= encrypt.c simon.c util.c
ENCRYPTOBJ 	= $(ENCRYPTSRC:%.c=%.o)

DECRYPTSRC 	= decrypt.c simon.c util.c
DECRYPTOBJ 	= $(DECRYPTSRC:%.c=%.o)

CC       	= clang
CFLAGS   	= -Wall -Wpedantic -Werror -Wextra

.PHONY: all clean

all: $(ENCRYPT) $(DECRYPT)

$(ENCRYPT): $(ENCRYPTOBJ)
	$(CC) -o $@ $^

$(DECRYPT): $(DECRYPTOBJ)
	$(CC) -o $@ $^

%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(ENCRYPT) $(DECRYPT) *.o
