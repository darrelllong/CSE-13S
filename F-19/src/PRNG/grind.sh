#!/bin/bash
n=1000000000
p=full
rm -f data.*
touch tests.txt
for i in {0,1,2,3,4,5,6,7,8,9}
do
	for j in {0,1,2,3,4,5,6,7,8,9}{0,1,2,3,4,5,6,7,8,9}
	do
		./$p -k -n $n -t -q > data.$i-$j &
	done
	  rm -f data.wait; ./$p -k -n $n -t -q > data.wait ; cat data.wait >> tests.txt ; rm -f data.wait
done
cat data.[0-9]* >> tests.txt
