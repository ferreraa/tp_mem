CC=gcc
CFLAGS=-Wall -g

alloc: alloc.o memshell.o
	gcc -o alloc.o memshell.o  alloc.h -Wall -W



memshell.o: memshell.c alloc.h
	gcc -o memshell.o -c memshell.c  -Wall


alloc.o: alloc.c alloc.h
	gcc -o alloc.o -c alloc.c   -Wall 






clean:
	rm -f alloc.o memshell.o  alloc
