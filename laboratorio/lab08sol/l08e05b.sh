#!/bin/bash

process=$1
time=$2

if [ -z $process ] || [ -z $time ]; then
  echo "Usage: es03 process time"
  return
fi

counter=0

while [ 1 -eq 1 ]; do
  line=$(ps -el | tr -s " " | cut -d " " -f 2,4,14 | grep $process | cut -d " " -f 1,2)
  if [ $? -eq 0 ]; then
    status=$(echo $line | cut -d " " -f 1)
    pid=$(echo $line | cut -d " " -f 2)
    echo "Checking status...$status"
    if [ $status = "Z" ]; then
      let "counter++"
      echo "Counter: $counter"
      if [ $counter -ge 5 ]; then
        kill -9 $pid
        echo "Killed $pid"
        return
      fi
    else
      let "counter=0"
    fi
    sleep $time
  fi
done
