#
# Makefile
#

target :
	echo "compiling my applications"
	gcc -Wall l02e01.c -o myLs



install : target
	mkdir bin
	mv myLs ./bin
	

clean :
	rm -f *.o

distclean : clean
	rm -R ./bin
