#!/bin/bash
for key in 64 128 256 384 512 1024 2048 4096
do
        echo "key is $key bits"
        ./keygen -v -b $key
        for i in {1..10}
        do
                echo "testing $i MB"
                trap "rm random.$i rand.test; exit" SIGINT
                dd if=/dev/random of=random.$i bs=1m count=$i
                ./encrypt -i random.$i | ./decrypt -o rand.test
                cmp random.$i rand.test
                if [[ $? -gt 0 ]]
                then
                        echo "Fail!"
                fi
                rm random.$i rand.test
        done
done
