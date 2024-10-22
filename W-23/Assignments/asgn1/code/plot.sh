#!/usr/bin/env bash

mkdir -p figures
rm -rf /tmp/monte_carlo* && make clean && make monte_carlo

echo -n "Plotting Monte Carlo Errors... "
mkdir -p /tmp/monte_carlo
for i in {0..5}; do
    ./monte_carlo -n 50000 -r $i > /tmp/monte_carlo/$i.dat
done

gnuplot <<EOF
set terminal epslatex
set output "figures/monte_carlo.tex"
set key autotitle columnhead
set grid
set xzeroaxis
set yrange [-1:1]
set logscale x 2
# set format x "2^{%L}"
set ylabel "Error"
set title "Monte Carlo Error Estimation"
offset=3.141592
FILES = system("ls -1 /tmp/monte_carlo/*.dat")
plot for [data in FILES] data using 1:(column(2)-offset) with lines linewidth 1 title ""
EOF
echo "done."

# Palette idea from Eugene!
echo -n "Plotting a grid"
mkdir -p /tmp/monte_carlo
./monte_carlo -n 1000 > /tmp/monte_carlo/grid.dat
gnuplot << EOF
    set terminal epslatex
    set output "figures/monte_carlo_grid.tex"
    set key autotitle columnhead
    set zeroaxis
    set tics scale 0
    set xrange [0:1]
    set yrange [0:1]
    set size square
    unset key
    unset colorbox
    set palette model RGB defined ( 0 'red', 1 'blue' )
    set object 1 circle back at 0,0 size 1
    plot "/tmp/monte_carlo/grid.dat" using 3:4:5 with points palette ps 0.8 pt 7
EOF