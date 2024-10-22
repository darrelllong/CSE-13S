#!/bin/bash
make clean
make
for i in sin cos tan aSin aCos aTan exp log root
do
    if [ ${i:0:1} == "a" ]
    then
        ./mathlib-test -${i:1:1} -P > /tmp/$i.dat
    else
        ./mathlib-test -${i:0:1} -P > /tmp/$i.dat
    fi
done
gnuplot <<XX
set terminal jpeg
set output "sin.jpg"
plot "/tmp/sin.dat" with linespoints title "sin(x)"
set output "cos.jpg"
plot "/tmp/cos.dat" with linespoints title "cos(x)"
set output "tan.jpg"
plot "/tmp/tan.dat" with linespoints title "tan(x)"
set output "sincos.jpg"
plot "/tmp/sin.dat" with linespoints title "sin(x)", "/tmp/cos.dat" with linespoints title "cos(x)"
set output "arcsin.jpg"
plot "/tmp/aSin.dat" with linespoints title "arcsin(x)"
set output "arccos.jpg"
plot "/tmp/aCos.dat" with linespoints title "arccos(x)"
set output "arctan.jpg"
plot "/tmp/aTan.dat" with linespoints title "arctan(x)"
set output "exp.jpg"
plot "/tmp/exp.dat" with linespoints title "exp(x)"
set output "log.jpg"
plot "/tmp/log.dat" with linespoints title "log(x)"
set output "sqr.jpg"
plot "/tmp/root.dat" with linespoints title "sqrt(x)"
XX
