find $1 -type f -exec grep -H -n "$2 *(.*)" \{} \; > tmp.txt
sort -t ':' -k 1 -k 2rn tmp.txt > $3
rm -f tmp.txt
