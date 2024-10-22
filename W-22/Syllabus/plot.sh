#!/usr/bin/env bash

cat > /tmp/tardiness.dat <<-EOS
Zero 100
One 85
Two 70
Three 55
Four 0
Five 0
Six 0
EOS

gnuplot <<EOF
    set terminal epslatex
    set output "figures/tardiness.tex"
    set xlabel "Days after due date"
    set ylabel "Maximum credit (\\\%)"
    set style data histogram
    set style fill solid
    set boxwidth 2
    plot "/tmp/tardiness.dat" using 2:xtic(1) title ""
EOF
