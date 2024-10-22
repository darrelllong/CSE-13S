#!/usr/bin/env zsh
for size in $(seq 128 128 4096)
do
for tries in {1..100}
do
        echo "$size `/usr/bin/time ./keygen -b $size |& awk '{print $1}'`"
done
done
