#!/bin/bash

assignment=$1                               # Assignment number.
executable=$2                               # Executable name.
file="studentlist.txt"                      # List of students.
output_file="$assignment/${assignment}.csv"  # Output CSV file.

# Check for missing script arguments.
if [ -z "$1" ]; then
    echo "Missing assignment number (asgnX) argument"
    exit 1
fi
if [ -z "$2" ]; then
    echo "Missing executable name argument"
    exit 1
fi
if [ ! -f "studentlist.txt" ]; then
    echo "Missing \"studentlist.txt\""
    exit 1
fi

# Create directory to put results in.
mkdir $assignment

# Create output CSV file and write some columns.
touch $output_file
echo "student, compiles, errors, warnings, makefile, clang_format, correct, writeup, design, readme" > $output_file

# Loop through each student in studentlist.txt.
# Clone each student's repo and run preliminary tests.
while IFS= read student
do
    # Init flags for assignment requirements.
    compiles="yes"
    errors="yes"
    warnings="no"
    makefile="yes"
    clang_format="yes"
    writeup="yes"
    design="yes"
    readme="yes"
    correct="yes"

    echo -e "\n=========================================================="

    # Clone the repo.
    git clone git@gitlab.soe.ucsc.edu:cse13s/fall19/$student.git $assignment/$student

    # Check if directory exists.
    if [ ! -d $assignment/$student ]; then
        echo "Failed to clone ${student}'s GitLab repository"
        continue
    fi
    pushd $assignment/$student/$assignment > /dev/null

    # Need to manually make first assignment.
    # Other assignments should have a Makefile.
    # Check if errors or warnings during compilation.
    if [ "$assignment" == "asgn0" ]; then
        out=$(clang -o hello hello.c)
        if [ $? -eq 0 ]; then
            errors="no"
        else
            if grep "warning:" <<< "${out}" > /dev/null; then
                warnings="yes"
            fi
        fi
    else
        out=$(make 2>&1)
        if [ $? -eq 0 ]; then
            errors="no"
        else
            if grep "warning:" <<< "${out}" > /dev/null; then
                warnings="yes"
            fi
        fi
    fi

    # Check if compilation was successful.
    if [ ! -f $executable ]; then
        compiles="no"
    fi
    echo "Compiles: $compiles"

    # Print out if warnings were thrown.
    echo "Warnings: $warnings"

    # Check if Makefile exists.
    if [ ! -f "Makefile" ]; then
        makefile="no"
    fi
    echo "Makefile: $makefile"

    # Check if write up exists.
    if [ ! -f "WRITEUP.pdf" ]; then
        writeup="no"
    fi
    echo "WRITEUP.pdf: $writeup"

    # Check if design doc exists.
    if [ ! -f "DESIGN.pdf" ]; then
        design="no"
    fi
    echo "DESIGN.pdf: $design"

    # Check if README exists.
    if [ ! -f "README.md" ]; then
        readme="no"
    fi
    echo "README: $readme"

    # Copy in clang-format file.
    cp ../../../cse13s.clang-format .clang-format

    # Check if each source file has been formatted.
    SOURCE=*.c
    for src in ${SOURCE}; do
        src_copy="${src}-copy.c"
        clang-format -style=file $src > $src_copy
        DIFF=$(diff $src $src_copy)
        if [ "$DIFF" ]; then
            clang_format="no"
            echo "$src wasn't formatted correctly"
        fi
        rm -f $src_copy
    done

    # Print success if no format differences found.
    echo "Source files formatted: $clang_format"

    # Check if header files, if they exist, have been formatted.
    if ls *.h 1> /dev/null 2>&1; then
        HEADER=*.h
        for hdr in ${HEADER}; do
            hdr_copy="${hdr}-copy.h"
            clang-format -style=file $hdr > $hdr_copy
            DIFF=$(diff $hdr $hdr_copy)
            if [ "$DIFF" ]; then
                clang_format="no"
                echo "$hdr wasn't formatted correctly"
            fi
            rm -f $hdr_copy
        done

        # Print success if no format differences found.
        echo "Header files formatted: $clang_format"
    fi

    # Run assignment's corresponding secondary script.
    cp ../../../secondary_scripts/$assignment/* .
    ./grade.sh

    # Check output.
    if [ $? -ne 0 ]; then
        correct="no"
    fi

    # Return to outer directory.
    popd > /dev/null

    # Add student's results into CSV.
    echo "$student, $compiles, $errors, $warnings, $makefile, $clang_format, $correct, $writeup, $design, $readme" >> $output_file
    echo -e "==========================================================\n"
done < $file

if [ "$(perl -v)" ]; then
    perl -pe 's/((?<=,)|(?<=^)),/ ,/g;' "$output_file" | column -t -s, | exec less  -F -S -X -K
    echo ""
fi

