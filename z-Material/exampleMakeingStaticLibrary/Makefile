all: add sub libAddSub testAddandSub

add: add.o
	gcc -g -c add.c

sub: sub.o
	gcc -g -c sub.c

libAddSub: add.o sub.o addSub.h
	ar rcs libAddSub.a add.o sub.o

testAddandSub: testAddandSub.c addSub.h
	gcc -g -o testAddandSub testAddandSub.c -L./ -lAddSub

