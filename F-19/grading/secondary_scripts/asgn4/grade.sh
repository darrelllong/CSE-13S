#!/bin/sh

STUDENT="student_"
CORRECT="correct_"

TEST_ONE="default_recurse.txt"
TEST_TWO="default_stacks.txt"
TEST_THREE="10_disks_recurse.txt"
TEST_FOUR="10_disks_stacks.txt"

tests_passed=0

timeout 10 ./tower -r > "${STUDENT}${TEST_ONE}"
if [ $? -ne 0 ]; then
    echo "Timeout for test 1!"
else
    if [ "$(diff "${STUDENT}${TEST_ONE}" "${CORRECT}${TEST_ONE}")" ]; then
        echo "Incorrect output for test 1!"
    else
        ((tests_passed=tests_passed+1))
    fi
fi

timeout 10 ./tower -s > "${STUDENT}${TEST_TWO}"
if [ $? -ne 0 ]; then
    echo "Timeout for test 2!"
else
    if [ "$(diff "${STUDENT}${TEST_TWO}" "${CORRECT}${TEST_TWO}")" ]; then
        echo "Incorrect output for test 2!"
    else
        ((tests_passed=tests_passed+1))
    fi
fi

timeout 10 ./tower -r -n 10 > "${STUDENT}${TEST_THREE}"
if [ $? -ne 0 ]; then
    echo "Timeout for test 6!"
else
    if [ "$(diff "${STUDENT}${TEST_THREE}" "${CORRECT}${TEST_THREE}")" ]; then
        echo "Incorrect output for test 3!"
    else
        ((tests_passed=tests_passed+1))
    fi
fi

timeout 10 ./tower -s -n 10 > "${STUDENT}${TEST_FOUR}"
if [ $? -ne 0 ]; then
    echo "Timeout for test 4!"
else
    if [ "$(diff "${STUDENT}${TEST_FOUR}" "${CORRECT}${TEST_FOUR}")" ]; then
        echo "Incorrect output for test 4!"
    else
        ((tests_passed=tests_passed+1))
    fi
fi

echo "Tests passed: $tests_passed/4"

if [ $tests_passed -ne 4 ]; then
    exit 1
fi

exit 0

