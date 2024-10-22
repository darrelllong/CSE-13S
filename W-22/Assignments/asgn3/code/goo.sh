#!/usr/bin/env bash
n=2
max=1000
echo what
while [[ $n -lt $max ]]
do
        echo $n
        n=`echo "$n+l($n)"|bc -l`
done
