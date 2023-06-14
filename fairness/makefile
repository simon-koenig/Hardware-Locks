# This is a comment line
CC=gcc

test: test.c
	${CC} test.c -o test -fopenmp -pedantic -Wall -Wextra -fsanitize=address -include ../locks/$(lock).c
	./test $(threads) $(repetitions) $(acquirees)  $(filename)
	rm test
	python3 PlottingScript.py $(lock)