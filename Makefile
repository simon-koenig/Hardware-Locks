.DEFAULT_GOAL := default_target

CC=gcc

CFLAGS=-pedantic -Wall -Wextra -fsanitize=address

# default values for variables
lock?=TestAndSetLock
thread_number?=8
array_size?=10000
acquisitions?=1000
reps?=10
server?=srun -p q_student --time=1:00 -N 1 -c 64

default_target:
	mkdir build

# run local by specifying variable 'server' as empty (server="")
correctnessTest: correctness/CorrectnessTest.c
	${CC} correctness/CorrectnessTest.c -o build/x -fopenmp -include locks/$(lock).c
	$(server) ./build/x $(thread_number) $(array_size) $(lock)

fairnessBench: benchmark/fairness.c
	${CC} benchmark/fairness.c -o build/x -fopenmp -include locks/$(lock).c
	$(server) ./build/x $(thread_number) $(reps) $(acquisitions)

latencyBench: benchmark/latency.c

low_thBench: benchmark/tp-low-contention.c

high_thBench: benchmark/tp-high-contention.c

# only for fairness so far
small-plot:
	python3 benchmark/PlottingScript.py $(lock)

# cleaning
.PHONY: clean
clean:
	@rm build/*