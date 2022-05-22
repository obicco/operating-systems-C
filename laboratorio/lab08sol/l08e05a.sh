#!/bin/bash


if [ $# -ne 2 ]
then
  echo "Usage: $0 process interval"
  exit 1
fi

is_zombie=0

while [ 1 -eq 1 ]
do

  line=`ps -el | grep $1`

  if [ $? -eq 0 ]
  then 
    pid=`echo $line | cut -d ' ' -f 2`
    state=`echo $line | cut -d ' ' -f 11`

    if [ $state == "Z" ] 
    then
      is_zombie=$(($is_zombie+1))
    fi

    if [ $is_zombie -ge 5 ]
    then
      kill -9 $pid
      echo "Process $pid killed"
      exit 0
    else
      is_zombie=0
    fi
  fi

  sleep $2

done
