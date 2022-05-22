#!/bin/bash

filename=$1
if [ -z $filename ]
then
  filename=/dev/stdin
fi

# Use an associative array to store word counts
# (Otherwise use two standard arrays)
# Define it for completeness
declare -A array

for word in $(cat $filename); do
  freq=${array[$word]}
  array[$word]=$((freq+1))
done

# array[*]  means: get all array values (i.e., element values)
# !array[*] means: get all array index values (i.e., index values)
for word in ${!array[*]}; do
  echo "$word ${array[$word]}"
done

# Delete the array
unset array
