#!/usr/bin/env bash

make clean && make

# Split each line using comma as the delimiter.
# Then extract x, the computed value, and the actual value from each line.
# Output to `sine()` results to `/tmp/sine.dat`.
# Output to `sin()` results to `/tmp/sin.dat`.
./sin-test | awk -F, '{
    split($1, computed, " ");
    split(computed[1], x, "[()]")
    split($2, library, " ");
    print x[2], computed[3] > "/tmp/computed.dat"
    print x[2], library[3] > "/tmp/library.dat"
}'

echo -n "Plotting... "
gnuplot <<EOF
set terminal pdf
set key outside
set zeroaxis

set output "comparison.pdf"
set title "sine() vs. sin()"
plot "/tmp/computed.dat" with linespoints title "sine()", \
    "/tmp/library.dat" with linespoints title "sin()"
EOF
echo "done."
