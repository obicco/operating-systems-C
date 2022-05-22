#!/bin/bash

if test $# -ne 3
then
  echo "Usage: $0 directory function file"
  exit 1
fi

grep -Hnr "$2 *(.*)" $1 | sort -t ':' -k 1 -k 2rn > $3
