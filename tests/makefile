# This is a comment line
CC=clang

test: test.c
	${CC} test.c -o test -fopenmp -include ../locks/$(lock).c
	./test
	rm test