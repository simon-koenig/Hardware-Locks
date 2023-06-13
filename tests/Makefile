# This is a comment line
CC=clang

test: Test.c
	${CC} Test.c -o test -fopenmp -include ../locks/$(lock).c
	./test
	rm test