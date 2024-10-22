#!/bin/bash

declare -a sorts=(
    "Aa"
    "Bb"
    "Cc"
    "Dd"
)

working_sorts=0

for s in "${sorts[@]}"
do
    STUDENT=student_sort${s:0:1}.txt
    STRIPPED=stripped_student_sort${s:0:1}.txt
    SORTED=sorted_student_sort${s:0:1}.txt

    timeout 10 ./sorting -${s:1:1} > $STUDENT

    if [ $? -ne 0 ]; then
        echo "Error for sort ${s:0:1}!"
    else
        # Get rid of the statistics in stripped file.
        sed -e '1,4d' $STUDENT > $STRIPPED

        # Convert all whitespace to newlines.
        perl -i -pe 's/\s+/\n/g' $STRIPPED

        # Delete blank lines.
        perl -i -n -e "print if /\S/" $STRIPPED

        # Sort numerically.
        sort -n $STRIPPED > $SORTED

        if [ "$(diff $STRIPPED $SORTED)" ]; then
            echo "Sort ${s:0:1} doesn't sort correctly."
        else
            echo "Sort ${s:0:1} sorts correctly."
            ((working_sorts=working_sorts+1))
        fi
    fi
done

echo "Working sorts: $working_sorts/4"

if [ $working_sorts -ne 4 ]; then
    exit 1
fi

exit 0
