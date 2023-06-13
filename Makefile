# This is a comment line
CC=gcc

correctnessTest: correctness/CorrectnessTest.c
	@echo "Testing correctness for $(lock):\n"
	@${CC} correctness/CorrectnessTest.c -o build/test -fopenmp -include locks/$(lock).c
	@./build/test $(thread_number) $(array_size)
	@rm build/test

.PHONY: clean

clean:
	rm build/*