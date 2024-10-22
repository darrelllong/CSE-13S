## Main Grading Script for CSE 13S

### Setup

You can do all your grading from inside this
directory. If you do insist on using a different directory for grading,
make sure all the contents of this directory ("grading") exists in the
directory you wish to grade in.

You will need a file called "studentlist.txt". Each line of this file should be
the CruzID of a student you are assigned to grade (just the CruzID, not email).

### Running the Script

To run the script: "./grading <assignment> <executable>"

The <assignment> argument is simply "asgnN" where N is the assignment number.
The <executable> field is the executable that is supposed to be generated after
successful compilation.

Example for assignment 0: "./grading.sh asgn0 hello"

### Cleaning Up

Simply run ./cleanup.sh <assignment> after running the above script.

This will delete all student repos pulled as well as the specified CSV file.

### Other Notes

- Make sure grading is done on the UCSC Unix Timeshare
- Always go through the students code, even if no errors are produced
- Check the output of the student's program, time permitting (don't take the
    grading script's report at face value)


