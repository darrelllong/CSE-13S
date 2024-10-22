#!/bin/bash

tests_passed=0

# Test if compression runs.
timeout 10 ./lzwcoder -c -i perfect.txt -o perfect.lzw
if [ $? -ne 0 ]; then
    echo "Error while compressing perfect.txt"
else
    ((tests_passed=tests_passed+1))
fi

# Test if decompression runs.
timeout 10 ./lzwcoder -d -i perfect.lzw -o perfect.out
if [ $? -ne 0 ]; then
    echo "Error while decompressing perfect.lzw"
else
    ((tests_passed=tests_passed+1))
fi

# Test if program can compress and decompress itself on short files.
if [ "$(diff perfect.txt perfect.out)" ]; then
    echo "Program can't decompress what it compresses for short files"
else
    ((tests_passed=tests_passed+1))
fi

# Test if program can compress and decompress itself on long files.
timeout 10 ./lzwcoder -c -i bible.txt -o bible.lzw
if [ $? -ne 0 ]; then
    echo "Error while compressing bible.txt"
fi

timeout 10 ./lzwcoder -d -i bible.lzw -o bible.out
if [ $? -ne 0 ]; then
    echo "Error while decompressing bible.lzw"
fi

if [ "$(diff bible.txt bible.out)" ]; then
    echo "Program can't decompress what it compresses for long files"
else
    ((tests_passed=tests_passed+1))
fi

# Test if program can read from stdin.
timeout 10 ./lzwcoder -c -o perfect.lzw < perfect.txt
if [ $? -ne 0 ]; then
    echo "Error while compressing perfect.txt from stdin"
else
    if [ -s perfect.lzw ]; then
        ((tests_passed=tests_passed+1))
    else
        echo "No output from compressing perfect.txt from stdin"
    fi
fi

# Test if program can write to stdout.
timeout 10 ./lzwcoder -c -i perfect.txt > perfect.out
if [ $? -ne 0 ]; then
    echo "Error while compressing perfect.txt to stdout"
else
    if [ -s perfect.out ]; then
        ((tests_passed=tests_passed+1))
    else
        echo "No output from compressing perfect.txt to stdout"
    fi
fi

echo "Tests passed: $tests_passed/6"

if [ $tests_passed -ne 4 ]; then
    exit 1
fi

exit 0
