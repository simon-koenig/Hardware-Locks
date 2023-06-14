#.DEFAULT_GOAL := default_target

CC=gcc

# default values for variables
lock?=TestAndSetLock
thread_number?=8
array_size?=10000
acquisitions?=1000
reps?=10

# compiling (with corresponding lock)

correctnessTest: correctness/CorrectnessTest.c
	@echo "Compiling correctnessTest.c for $(lock)..."
	@${CC} correctness/CorrectnessTest.c -o build/x -fopenmp -include locks/$(lock).c
	@echo "Done.\nExecute with 'make execute thread_number=... array_size=...'"
	@echo "On server execute with 'make execute_server ...'"

fairnessBench: benchmark/fairness.c
	@echo "Compiling fairness.c for $(lock)..."
	@${CC} benchmark/fairness.c -o build/x -fopenmp -include locks/$(lock).c
	@echo "Done.\nExecute with 'make execute thread_number=... reps=... acquisitions=... filename=...'"
	@echo "On server execute with 'make execute_server ...'"

latencyBench: benchmark/fairness.c
	@echo "Compiling fairness.c for $(lock)..."
	@${CC} benchmark/fairness.c -o build/x -fopenmp -include locks/$(lock).c
	@echo "Done.\nExecute with 'make execute thread_number=... reps=... acquisitions=... filename=...'"
	@echo "On server execute with 'make execute_server ...'"

low_thBench: benchmark/fairness.c
	@echo "Compiling fairness.c for $(lock)..."
	@${CC} benchmark/fairness.c -o build/x -fopenmp -include locks/$(lock).c
	@echo "Done.\nExecute with 'make execute thread_number=... reps=... acquisitions=... filename=...'"
	@echo "On server execute with 'make execute_server ...'"

high_thBench: benchmark/fairness.c
	@echo "Compiling fairness.c for $(lock)..."
	@${CC} benchmark/fairness.c -o build/x -fopenmp -include locks/$(lock).c
	@echo "Done.\nExecute with 'make execute thread_number=... reps=... acquisitions=... filename=...'"
	@echo "On server execute with 'make execute_server ...'"

# for local execution
execute: build/x
	@./build/x $(thread_number) $(array_size) $(reps) $(acquisitions)
#python3 benchmark/PlottingScript.py $(lock)

# for execution on server compute node
execute_server: build/x
	@srun -p q_student --time=1:00 -N 1 -c 64 ./build/x $(thread_number) $(array_size) $(reps) $(acquisations)

# cleaning
.PHONY: clean
clean:
	@rm build/*