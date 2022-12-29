#! /bin/bash
#To make it executable --> chmod +x helloScript.sh

#Output to terminal
echo "hello bash script"

#Output to file
echo "hello bash script" > file.txt

#Deliberate input from terminal to file
cat > file.txt  #Replaces the file content
cat >> file.txt #Appends the file content

#Tells that the world creative is a document end of line kind of and execute the command until reach it
cat << creative
this is hello creative text
add another line
creative

#If statement
count = 10

#Inside if brackets always leave a space at the beginning and at the end
if [ $count -eq 10 ] #eq = equal, ne = not equal, gt = greater than
#you can use a more arythmetic --> if (( $count > 9 ))
then
    echo "first condition is true"
elif (( $count < 0 ))
then
    echo "second condition is true"
else
    echo "the condition is false"
fi

age=10

#Can be used --> if [[ "$age" -gt 18 && "$age" -lt 40 ]] 
if [ "$age" -gt 18 ] && [ "$age" -lt 40 ] 
then
    echo "Age is correct"
else
    echo "Age is not correct"
fi

if [[ "$age" -gt 18 || "$age" -lt 40 ]] 
then
    echo "Age is correct"
else
    echo "Age is not correct"
fi

#LOOPS
number=1

while [ $number -lt 10 ]  #can use keyword until
do
    echo "$n"
    number=$(( n+1 ))
done

for i in 1 2 3 4 5
do 
    echo $i
done

for i in (0..20)
do
    echo $i
done

for (( i=0; i<5; i++))
do
    echo $i
done

#$0 is the first argument if I have more arguments to the .sh launched in the terminal I can handle them in order like:
echo $0 $1 $2 $3

#To get an undefined number of inputs I can declare tham as:
args=("$0")

echo $(args[0]) $(args[1]) $(args[2]) #as defined array output or
echo $0 #print all of them

#Script input
while read line
do
    echo "$line"
done < "$(1:-/dev/stdin)"

#Standard output and Standard error
ls -al 1>file1.txt 2>file2.txt

#SEND OUTPUT FROM ONE SCRIPT TO ANOTHER SCRIPT THROUGH PIPES
MESSAGE="Hello audience"
export MESSAGE
./secondscript.sh

#STRING PROCESSING
echo "Enter first string"
read st1

echo "Enter second string"
read st2

if [ "$st1" == "$st2" ] # \< for is smaller or \> is bigger
then
    echo "Strings match"
else
    echo "Strings don't match"
fi

#concatenation
c=$st1$st2
echo $c

echo $(st1^) #lower case
echo $(st1^^) #upper case

#NUBERS AND ARITHMETIC
