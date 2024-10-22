#!/bin/bash

declare -a assignments=(
    "asgn0"
    "asgn1"
    "asgn2"
    "asgn3"
    "asgn4"
    "asgn5"
    "asgn6"
    "asgn7"
    "asgn8"
)

# Remove assignment grading directories if they exist.
for asgn in "${assignments[@]}"
do
    if [ -d "$asgn" ]; then
        echo "Deleting $asgn directory"
        rm -rf $asgn
    fi
done
