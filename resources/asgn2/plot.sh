#!/usr/bin/env bash

# Example data is sent to /tmp/output.
cat > /tmp/output <<herestr
sqrt(1 - x^4),0.000000,1.000000,100
2,0.812163891034571
4,0.852988388966857
6,0.862714108378597
8,0.866720323920874
10,0.868814915051592
12,0.870069316562543
14,0.870889853274920
herestr

# Get the title from the output.
title=$(head -n 1 /tmp/output | awk -F, '{ print $1 }')

# Put all but first line of output in data file.
tail -n +2 /tmp/output > /tmp/data

# Plot the data, specifying that commas are used as the delimiter.
gnuplot <<EOF
    set terminal pdf
    set output "example.pdf"
    set datafile separator ","
    set xlabel "x"
    set ylabel "y"
    set title "$title"
    plot "/tmp/data" with linespoints
EOF
