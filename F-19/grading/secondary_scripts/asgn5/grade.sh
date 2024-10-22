#!/bin/sh

# Get output of student program.
./factor > student_output.txt

# Grab any differences.
DIFF=$(diff student_output.txt correct_output.txt)

# Print differences if they exist.
if [ "$DIFF" ]; then
    echo "Output: incorrect"
    echo "Student output on left, correct output on right."
    echo "$DIFF"
    exit 1
else
    echo "Output: correct"
fi

exit 0

