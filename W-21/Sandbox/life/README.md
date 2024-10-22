## Conway's Game of Life

A C program using `ncurses` to simulate Conway's game of life.


#### Building

```
$ make
```

or

```
$ make all
```


#### Running

```
$ ./life [-ts] [-n ITERATIONS] [-i INPUT] [-o OUTPUT]
```


#### Options

- `-t`: Enable toroidal flag (life forms wrap around)
- `-s`: Enable silence (no `ncurses` display)
- `-n ITERATIONS`: Specify number of life iterations
- `-i INPUT`: Specify input containing the grid to parse (defualt: `stdin`)
- `-o OUTPUT`: Specify output to print final grid to (default: `stdout`)
