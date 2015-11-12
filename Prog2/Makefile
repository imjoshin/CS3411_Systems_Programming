test	:libDFile.a test.c print.c
		gcc -g -o test test.c print.c libDFile.a

libDFile.a	: dopen.o dclose.o dalloc.o dfree.o
		rm -f libDFile.a
		ar -cq libDFile.a dopen.o dclose.o dalloc.o dfree.o

dopen.o		: dfile.h dopen.c
		gcc -g -std=c99 -c dopen.c

dclose.o	: dfile.h dclose.c
		gcc -g -std=c99 -c dclose.c

dalloc.o	: dfile.h dalloc.c
		gcc -g -std=c99 -c dalloc.c

dfree.o		: dfile.h dfree.c
		gcc -g -std=c99 -c dfree.c

clean:
		rm -f libDFile.a dopen.o dclose.o dalloc.o dfree.o test

run:
		make clean
		make
		./test
