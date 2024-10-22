CC = clang
CFLAGS = -Wall -Werror -Wextra -Wpedantic

all: dfs bfs topo

dfs: dfs.o stack.o graph.o
	$(CC) -o $@ $^

bfs: bfs.o queue.o graph.o
	$(CC) -o $@ $^

topo: topo.o stack.o ll.o node.o graph.o
	$(CC) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f dfs bfs topo *.o
