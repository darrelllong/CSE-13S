#!/bin/bash

# Rebuild the sincos executable first.
make clean && make sincos

# Run the sincos executable, redirecting its output.
./sincos > /tmp/sin.dat

# This is the heredoc that is sent to gnuplot.
gnuplot <<END
    set terminal epslatex
    set output "figures/sinplot.tex"
    set xlabel "\$x$"
    set ylabel "\$y$"
    set zeroaxis
    set title "\$y = \\\sin(x)$"
    plot "/tmp/sin.dat" with lines title ""
END

echo "Done plotting!"
