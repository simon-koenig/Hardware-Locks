#.DEFAULT_GOAL := default_target

CC=gcc

# default values for variables

lock?=TestAndSetLock
thread_number?=8
array_size?=10000
filename?=lock

# compilation (with corresponding lock)
correctnessTest: correctness/CorrectnessTest.c
	@echo "Compiling correctnessTest for $(lock)..."
	@${CC} correctness/CorrectnessTest.c -o build/x -fopenmp -include locks/$(lock).c
	@echo "Done.\nExecute with 'make execute thread_number=... array_size=...'"

# for local execution
execute: build/x
	@./build/x $(thread_number) $(array_size) $(filename) $(reps)

# for execution on compute node
execute_server: build/x
	@srun -p q_student --time=1:00 -N 1 -c $(thread_number) ./build/x $(thread_number) $(array_size) $(filename) $(reps)

.PHONY: clean

clean:
	@rm build/*