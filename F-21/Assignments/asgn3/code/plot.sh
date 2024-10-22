#!/usr/bin/env bash

rm -f /tmp/{heap,insertion,quick,shell}.dat

for i in {0..5}; do
    awk -F, '{
        split($1,name," ");
        split($2,elements," ");
        split($3,moves," ");
        split($4,compares," ");
        file = sprintf("/tmp/%s.dat", tolower(name[1]));
        printf "%d %d %d\n", elements[1], moves[1], compares[1] >> file
    }' <(./sorting -a -n $(( 10**i )) -p 0)
done

gnuplot <<EOF
set terminal pdf
set bmargin 4
set key outside
set size ratio 0.75
set xlabel "Elements"

set output "moves.pdf"
set title "Moves Performed"
set ylabel "Moves"
plot "/tmp/heap.dat" using 1:2 with linespoints title "Heap Sort", \
    "/tmp/insertion.dat" using 1:2 with linespoints title "Insertion Sort", \
    "/tmp/quick.dat" using 1:2 with linespoints title "Quick Sort", \
    "/tmp/shell.dat" using 1:2 with linespoints title "Shell Sort"

set output "compares.pdf"
set title "Comparisons Performed"
set ylabel "Comparisons"
plot "/tmp/heap.dat" using 1:3 with linespoints title "Heap Sort", \
    "/tmp/insertion.dat" using 1:3 with linespoints title "Insertion Sort", \
    "/tmp/quick.dat" using 1:3 with linespoints title "Quick Sort", \
    "/tmp/shell.dat" using 1:3 with linespoints title "Shell Sort"
EOF
