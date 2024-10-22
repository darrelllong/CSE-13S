CC          = clang
CFLAGS      = -Wall -Werror -Wextra -Wpedantic `pkg-config --cflags gmp`
LFLAGS      = `pkg-config --libs gmp`
TARGETS     = keygen encrypt decrypt
TARGOBJ     = $(patsubst %,%.o,${TARGETS})
MODULES     = ss randstate numtheory
MODOBJ      = $(patsubst %,%.o,${MODULES})

all: CFLAGS+=-O3
all: LFLAGS+=-s
all: $(TARGETS)

debug: CFLAGS+=-g
debug: $(TARGETS)

$(TARGETS): %: %.o $(MODOBJ)
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean: tidy
	$(RM) $(TARGETS)

tidy:
	$(RM) $(MODOBJ) $(TARGOBJ)

cleankeys:
	$(RM) *.{pub,priv}

format:
	clang-format -i -style=file *.[ch]
