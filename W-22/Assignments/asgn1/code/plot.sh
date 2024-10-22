#!/usr/bin/env bash

mkdir -p figures
rm -f /tmp/collatz_*.dat && make clean && make

echo -n "Plotting Collatz sequence lengths... "
for i in {2..10000}; do
    length=$(./collatz -n $i | wc -l)
    echo "$i $length" >> /tmp/collatz_length.dat
done

gnuplot <<EOF
set terminal epslatex
set output "figures/collatz_length.tex"
set xlabel "\$n$"
set ylabel "length"
set style line 5 lt rgb "plum" ps 0.1 pt 6
set title "Collatz Sequence Lengths"
plot "/tmp/collatz_length.dat" with points ls 5 title ""
EOF
echo "done."

echo -n "Plotting maximum Collatz sequence values... "
for i in {2..10000}; do
    max=$(./collatz -n $i | sort -rn | head -n 1)
    echo "$i $max" >> /tmp/collatz_maxval.dat
done

gnuplot <<EOF
set terminal epslatex
set output "figures/collatz_maxval.tex"
set zeroaxis
set xlabel "\$n$"
set ylabel "value"
set yrange [0:100000]
set style line 5 lt rgb "plum" ps 0.1 pt 6
set title "Maximum Collatz Sequence Value"
plot "/tmp/collatz_maxval.dat" with points ls 5 title ""
EOF
echo "done."

echo -n "Plotting Collatz sequence length histogram... "
awk '{ print $2 }' /tmp/collatz_length.dat | sort -n | uniq -c > /tmp/collatz_hist.dat

gnuplot <<EOF
set terminal epslatex
set output "figures/collatz_hist.tex"
set zeroaxis
set xlabel "length"
set ylabel "frequency"
set xtics 25
set xrange [0:*]
set style data histogram
set style fill solid
set boxwidth 0.9
set title "Collatz Sequence Length Histogram"
plot "/tmp/collatz_hist.dat" using 1 linecolor "plum" title ""
EOF
echo "done."
