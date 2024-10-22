#!/usr/bin/env bash

tests=(
    'a,0.0,1.0,100'
    'b,1.01,2.0,100'
    'c,-10.0,10.0,100'
    'd,-3.14,3.14,100'
    'e,-3.14,3.14,100'
    'f,1.01,10.0,100'
    'g,-3.14,3.14,100'
    'h,1.0,10.0,100'
    'i,0.0,1.0,100'
)

for t in "${tests[@]}"; do
    while IFS=',' read -r fn p q n; do
        echo -n "Plotting $fn... "

        title=$(./integrate -$fn -p $p -q $q -n $n | head -n 1 | awk -F, '{ print $1 }')
        ./integrate -$fn -p $p -q $q -n $n | tail -n +2 > /tmp/$fn.dat

gnuplot <<EOF
    set terminal pdf
    set zeroaxis
    set datafile separator ","
    set xlabel "Partitions"
    set ylabel "Sum"
    set output "$fn.pdf"
    set title "$title"
    plot "/tmp/$fn.dat" using 1:2 with lines title "[$p, $q]"
EOF

        echo "done."
    done <<< $t
done

