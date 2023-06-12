# This is a comment line
CC=gcc

test: test.c
	${CC} test.c -o test -fopenmp -pedantic -Wall -Wextra -fsanitize=address -include ../locks/$(lock).c
	./test $(threads) $(acquirees) $(repetitions) $(filename)
	rm test