.DEFAULT_GOAL := default_target
CC = gcc
CFLAGS = -pedantic -Wall -Wextra -fsanitize=address -lm
SRC_DIR = src
MKDIR ?= @mkdir
BUILD_DIR = build
DATA_DIR = data

# Default values for variables
LOCKS ?= BaselineLock TestAndSetLock TestAndTestAndSetLock TicketLock ArrayLock CLHLock MCSLock HemLock
sampleSize ?= 1000
repetitions ?= 10
threads ?= 16
server?=srun -p q_student --time=1:00 -N 1 -c 64

$(DATA_DIR):
	@echo "Creating data directory: $(DATA_DIR) with subdirectories"
	$(MKDIR) $(DATA_DIR)
	$(MKDIR) $(DATA_DIR)/latency
	$(MKDIR) $(DATA_DIR)/throughput
	$(MKDIR) $(DATA_DIR)/correctness
	$(MKDIR) $(DATA_DIR)/fairness


$(BUILD_DIR):
	@echo "Creating build directory: $(BUILD_DIR)"
	$(MKDIR) $(BUILD_DIR)


default_target:
	mkdir build

# run local by specifying variable 'server' as empty (server="")
correctnessTest: ./correctness/CorrectnessTest.c  $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		${CC} ./correctness/CorrectnessTest.c -o $(BUILD_DIR)/x -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		$(server) ./$(BUILD_DIR)/x $(threads) $(sampleSize) $$lock ; \
	done



fairnessBench: benchmark/fairness.c  $(BUILD_DIR) $(DATA_DIR)
	${CC} benchmark/fairness.c -o build/x -fopenmp -include locks/$(lock).c
	$(server) ./build/x $(thread_number) $(reps) $(acquisitions)


latency: ./benchmark/latency.c  $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		echo "Measuring Latency Lock= $$lock \n"  ; \
		${CC} ./benchmark/latency.c -o $(BUILD_DIR)/$@ -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		./$(BUILD_DIR)/$@ $(threads) $(sampleSize) $(repetitions) $$lock ; \
	done 

tp-high-contention: ./benchmark/tp-high-contention.c $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		echo "Measuring throughput high contention. Lock= $$lock \n" ; \
		${CC} ./benchmark/tp-high-contention.c -o $(BUILD_DIR)/$@ -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		./$(BUILD_DIR)/$@ $(threads) $(sampleSize) $(repetitions) $$lock ; \
	done
	
tp-low-contention: ./benchmark/tp-low-contention.c $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		echo "Measuring throughput low contention. Lock= $$lock \n" ; \
		${CC} ./benchmark/tp-low-contention.c -o $(BUILD_DIR)/$@ -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		./$(BUILD_DIR)/$@ $(threads) $(sampleSize) $(repetitions) $$lock ; \
	done 


plotTPHigh: 
	python3 plotTP.py high

plotTPLow:
	python3 plotTP.py low

plotLatency:
	python3 plotLatency.py

# only for fairness so far
small-plot:
	python3 benchmark/PlottingScript.py $(lock)

# cleaning
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
