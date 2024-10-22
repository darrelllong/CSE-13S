CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic -g
LFLAGS = -lgmp

INC = /opt/homebrew/include /usr/local/include
INC_PARAMS = $(INC:%=-I%)

LIB = /opt/homebrew/lib /usr/local/lib
LIB_PARAMS = $(LIB:%=-L%)

all: keygen encrypt decrypt primetest

keygen: keygen.o rsa.o
	$(CC) -o $@ $^ $(LIB_PARAMS) $(LFLAGS)

encrypt: encrypt.o rsa.o
	$(CC) -o $@ $^ $(LIB_PARAMS) $(LFLAGS)

decrypt: decrypt.o rsa.o
	$(CC) -o $@ $^ $(LIB_PARAMS) $(LFLAGS)

primetest: primetest.o rsa.o
	$(CC) -o $@ $^ $(LIB_PARAMS) $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INC_PARAMS) -c $<

clean:
	rm -f keygen encrypt decrypt primetest *.o
