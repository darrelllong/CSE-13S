#!/bin/sh

if [ "$(./hello)" != "Hello World!" ]; then
    echo "Output: incorrect"
    exit 1
else
    echo "Output: correct"
    exit 0
fi

