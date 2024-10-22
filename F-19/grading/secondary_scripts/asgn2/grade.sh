#!/bin/bash

# Create some test inputs.
for i in {1..5}
do
    # Touch the file.
    OUT="test${i}.txt"
    touch $OUT

    # Add random seed to file.
    echo "$RANDOM" > $OUT

    # Add number of players to file
    echo "$(((RANDOM%9+2)))" >> $OUT
done

# Counter to check how many tests student passed.
tests_passed=0

# For each test, check if student output is correct.
for i in {1..5}
do
    TEST="test${i}.txt"
    CORRECT="test${i}_correct.txt"
    STUDENT="test${i}_student.txt"

    # Get correct output.
    ./correct_lrc < $TEST > $CORRECT

    # Give 10 seconds to get student output.
    timeout 10 ./lrc < $TEST > $STUDENT

    # Check if timeout occurred.
    if [ $? -ne 0 ]; then
        echo "Timeout for test ${i}"
    else
        # Strip ending whitespace from student output (to be nice).
        sed -i 's/\s*$//' $STUDENT

        # Check for any differences
        if [ "$(diff $CORRECT $STUDENT)" ]; then
            echo "Incorrect output for test ${i}"
        else
            ((tests_passed=tests_passed+1))
        fi
    fi
done

echo "Tests passed: $tests_passed/5"

if [ $tests_passed -ne 5 ]; then
    exit 1
fi

exit 0

