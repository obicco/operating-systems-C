#
# Makefile
#

target :
	echo "compiling my applications"
	gcc -Wall l02e01.c -o myLs.o



install : target
	mkdir bin
	mv myLs.o ./bin
	

clean :
	rm -f *.o

distclean : clean
	rm -R ./bin

