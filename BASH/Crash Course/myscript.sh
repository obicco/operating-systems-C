#! /usr/bin/bash

# ECHO COMMAND
echo Hello World!

# VARIABLES
# Uppercase by convention
# Letters, numbers, underscores
NAME="Bicco"
echo "My name is $NAME"

# USER INPUT
read -p "Enter you name: " NAME
echo "Hello $NAME, nice to meet you"

# IF STATEMENT
if [ "$NAME" == "Oskar" ]
then
    echo "Your name is Oskar"
else 
    echo "Your name is not Oskar"
fi #fi to finish the if statement

# ELSE-IF (elif)
if [ "$NAME" == "Oskar" ]
then
    echo "Your name is Oskar"
elif [ "$NAME" == "Bicco" ]
then
    echo "Cool name!"
else 
    echo "Your name is not Oskar"
fi #fi to finish the if statement

# COMPARISON

#########
# val1 -eq val 2 Return true if the values are equal
# val1 -ne val 2 Return true if the values are not equal
# val1 -gt val 2 Return true if val1 is greater than val2
# val1 -ge val 2 Return true if val1 is greater than or equal to val2
# val1 -lt val 2 Return true if val1 is less than val2
# val1 -le val 2 Return true if is less than or equal to val2
#########

NUM1=3
NUM2=5
if [ "$NUM1" -gt "$NUM2" ]
then
    echo "$NUM1 is greater than $NUM2"
else
    echo "$NUM1 is less than $NUM2"
fi

# FILE CONDITIONS
FILE="test.txt"
if [ -f "$FILE" ]
then
    echo "$FILE is a file"
else
    echo "$FILE is NOT a file"
fi

#######
# -d file True if the file is a directory
# -e file True if the file exists (note that this is not particularly portable, thus -f is generally used)
# -f file True if the providd string is a file
# -g file True if the group id is set on a file
# -r file True if the file is readble
# -s file True if the file has a non-zero size
# -u file True if the user id is set on a file
# -w file True if the file is writable
# -x file True if the file is not executable
#######

# CASE STATEMENT
read -p "Are you 21 or over? Y/N " ANSWER
case "$ANSWER" in
    [yY] | [yY][eE][sS])
        echo "You can have a beer :)"
        ;;
    [nN] | [nN][oO])
        echo "Sorry, no drinking"
    ;;
*)#default
    echo "Please enter y/yes or n/no"
    ;;
esac

# SIMPLE FOR LOOP
NAMES="Bread Kevin Alice Mark"
for NAME in $NAMES
    do 
        echo "Hello $NAME"
done

# FOR LOOP TO RENAME FILES
FILES=$(ls *.txt)
NEW="new"
for FILE in $FILES
    do
        echo "Renaming $FILE to new new-$FILE"
        mv $FILE $NEW-$FILE
done

# WHILE LOOP - READ THROUGH A FILE LINE BY LINE
LINE=1
while read -r CURRENT_LINE
    do 
        echo "$LINE: $CURRENT_LINE"
        ((LINE++))
done < "./new-1.txt"

# FUNCTION
function sayHello() {
    echo "Hello World"
}

sayHello

# FUNCTION WITH PARAMS
function greet() {
    echo "Hello, I am $1 and I am $2"
}

greet "Oskar" "26"

# CREATE FOLDER AND WRITE TO A FILE
mkdir hello
touch "hello/world.txt"
echo "Hello World" >> "hello/world.txt"
echo "Created hello/world.txt"

