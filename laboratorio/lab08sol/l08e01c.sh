#!/bin/bash

if test $# -ne 3
then
  echo "Usage: $0 directory function file"
  exit 1
fi

for f in `find $1 -type f`
do
  grep -H -n "$2 *(.*)" $f | sort -r -t ':' -k 1 -k 2n > $3  
done
