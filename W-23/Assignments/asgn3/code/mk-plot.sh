#!/usr/bin/env bash
pid=$$
rm -f /tmp/cmp.* /tmp/means.* /tmp/c.*
for s in b q h
do
for ((i=10; i<=1000001; i+=2500))
do
for r in {1..10}
do
pid=$((pid+1))
./sorting -$s -r $pid -n $i -p 0 | awk '/elements/ { print $7 }' >> /tmp/cmp.$s.$i
done
echo -n "$i " >> /tmp/means.$s
awk -f mean.awk < /tmp/cmp.$s.$i >> /tmp/means.$s
rm -f /tmp/cmp.$s.*
done
done
for s in b q h
do
awk '{ x=$2/(log($1+1)*$1); print($1, x) }' < /tmp/means.$s > /tmp/c.$s
done
gnuplot <<FUBAR
set terminal jpeg
set output "compares.jpg"
set title "Estimated c = sort(n)/(n log(n))"
set ylabel "c"
plot "/tmp/c.h" using 1:2 with lines title "Heap Sort", \
    "/tmp/c.b" using 1:2 with lines title "Batcher Sort", \
    "/tmp/c.q" using 1:2 with lines title "Quick Sort"
FUBAR
