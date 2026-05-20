connectn.out : main.o connectn.o
	gcc main.o connectn.o -o connectn.out

main.o : main.c  connectn.h
	gcc -Wall -c main.c 

connectn.o : connectn.c connectn.h
	gcc -Wall -c connectn.c 

clean :
	rm *.o connectn.out