#!/bin/bash

file="studentlist.txt"

# Loop through each student in studentlist.txt.
# Clone each student's repo and run preliminary tests.
while IFS= read student
do
    if [ ! -d "students/$student" ]; then
        git clone git@gitlab.soe.ucsc.edu:cse13s/winter20/$student.git $assignment/$student
    fi

    if [ -d "students/$student" ]; then
        pushd students/$student > /dev/null
        git pull
        popd > /dev/null
    else
        echo "Couldn't clone or pull $student's directory."
    fi
done < $file

