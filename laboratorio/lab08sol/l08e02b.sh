#!/bin/bash
i=0
max=0
aux=0

while read var
do
  aux=${#var}
  if test $aux -gt $max 
  then
    max=$aux		
  fi	
  i=$((i+1))
done < $1 

echo "Lunghezza massima della stringa"$max
echo "Lunghezza del file "$i" righe"
