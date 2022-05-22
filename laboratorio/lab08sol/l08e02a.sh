#!/bin/bash

if [ $# -ne 1 ]
then
  echo "Usage: $0 file"
  exit 1
fi

for i in `cat $1`
do
  echo $i | wc -c >> tmp.txt
done

echo `sort -r -n tmp.txt | head -n 1`
echo `wc -l $1`
