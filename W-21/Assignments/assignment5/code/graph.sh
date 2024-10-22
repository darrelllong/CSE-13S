#!/usr/bin/env bash
SMALL=15000
LARGE=1000000
if [ ! -e sorting ]
then
    make
fi
for s in b
do
    touch sort-$s.data
    for ((i = 1; i < SMALL; i = i * 2))
    do
        echo -n "sorting -$s -n $i"
        /usr/bin/time ./sorting -p 0 -n $i -$s | awk '/elements/ { print $1, $3, $5 }' >> sort-$s.data
    done
    awk '{ print $1, $3}' < sort-$s.data > sort-$s.cmp
	awk '{ print $1, $2}' < sort-$s.data > sort-$s.mov
	rm sort-$s.data
done
for s in s q h
do
    touch sort-$s.data
    for ((i = 1; i < LARGE; i = i * 2))
    do
	    echo -n "sorting -$s -n $i"
        /usr/bin/time ./sorting -p 0 -n $i -$s | awk '/elements/ { print $1, $3, $5 }' >> sort-$s.data
    done
    awk '{ print $1, $3}' < sort-$s.data > sort-$s.cmp
	awk '{ print $1, $2}' < sort-$s.data > sort-$s.mov
	rm sort-$s.data
done
gnuplot << XXX
set xzeroaxis
set yzeroaxis
set tics out nomirror
set tics scale 0.8
set key out
set terminal png
set output "sort-cmp.png"
set xlabel "Elements"
set ylabel "Comparisons"
set xrange [0:$LARGE]
set logscale x
plot "sort-s.cmp" smooth sbezier lw 3 lc rgb "red" title "Shellsort", \
"sort-q.cmp" smooth sbezier lw 3 lc rgb "blue" title "Quicksort", \
"sort-b.cmp" smooth sbezier lw 3 lc rgb "green" title "Bubblesort", \
"sort-h.cmp" smooth sbezier lw 3 lc rgb "orange" title "Heapsort"
XXX
gnuplot << XXX
set xzeroaxis
set yzeroaxis
set tics out nomirror
set tics scale 0.8
set key out
set terminal png
set output "sort-mov.png"
set xlabel "Elements"
set ylabel "Moves"
set xrange [0:$LARGE]
set logscale x
plot "sort-s.mov" smooth sbezier lw 3 lc rgb "red" title "Shellsort", \
"sort-q.mov" smooth sbezier lw 3 lc rgb "blue" title "Quicksort", \
"sort-b.mov" smooth sbezier lw 3 lc rgb "green" title "Bubblesort", \
"sort-h.mov" smooth sbezier lw 3 lc rgb "orange" title "Heapsort"
XXX
rm -f sort-?.{cmp,mov}
