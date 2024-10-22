#!/bin/bash

make clean && make sincos   # Rebuild the sincos executable.
./sincos > /tmp/sin.dat     # Place the data points into a file.

# This is the heredoc that is sent to gnuplot.
gnuplot <<END
    set terminal pdf
    set output "sin.pdf"
    set title "sin(x)"
    set xlabel "x"
    set ylabel "y"
    set zeroaxis
    plot "/tmp/sin.dat" with lines title ""
END
