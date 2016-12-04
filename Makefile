CC=gcc
CFLAGS=-Wall -g

alloc: alloc.o memshell.o
	gcc -o alloc alloc.o memshell.o

memshell.o: memshell.c alloc.h
	gcc -o memshell.o -c memshell.c


alloc.o: alloc.c alloc.h
	gcc -o alloc.o -c alloc.c








clean:
	rm -f alloc.o memshell.o  alloc
