#!/bin/sh

if [ -z "$1" ]; then
    echo "Missing csv to prettyprint"
    exit 1
fi

perl -pe 's/((?<=,)|(?<=^)),/ ,/g;' $1 | column -t -s, | exec less  -F -S -X -K
