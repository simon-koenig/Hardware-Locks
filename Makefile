.DEFAULT_GOAL := default_target
CC = gcc
CFLAGS = -lm #-pedantic -Wall -Wextra -fsanitize=address -lm
SRC_DIR = src
MKDIR ?= @mkdir
BUILD_DIR = build
DATA_DIR = data
PLOTS_DIR = plots

# Default values for variables
LOCKS ?= BaselineLock TestAndSetLock TestAndTestAndSetLock TicketLock ArrayLock CLHLock MCSLock HemLock
sampleSize ?= 10000
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

$(PLOTS_DIR):
	@echo "Creating plots directory: $(PLOTS_DIR)"
	$(MKDIR) $(PLOTS_DIR)


default_target:
	mkdir build

# run local by specifying variable 'server' as empty (server="")
correctnessTest: ./correctness/CorrectnessTest.c  $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		${CC} ./correctness/CorrectnessTest.c -o $(BUILD_DIR)/x -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		$(server) ./$(BUILD_DIR)/x $(threads) $(sampleSize) $$lock ; \
	done

fairness: benchmark/fairness.c  $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		${CC} benchmark/fairness.c -o build/x -fopenmp -include locks/$$lock.c ; \
		$(server) ./build/x $(threads) $(repetitions) $(sampleSize) $$lock; \
	done

latency: ./benchmark/latency.c  $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		echo "Measuring Latency Lock= $$lock \n"  ; \
		${CC} ./benchmark/latency.c -o $(BUILD_DIR)/$@ -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		$(server) ./$(BUILD_DIR)/$@ $(threads) $(sampleSize) $(repetitions) $$lock ; \
	done 

tp-high-contention: ./benchmark/tp-high-contention.c $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		echo "Measuring throughput high contention. Lock= $$lock \n" ; \
		${CC} ./benchmark/tp-high-contention.c -o $(BUILD_DIR)/$@ -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		$(server) ./$(BUILD_DIR)/$@ $(threads) $(sampleSize) $(repetitions) $$lock ; \
	done

tp-low-contention: ./benchmark/tp-low-contention.c $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		echo "Measuring throughput low contention. Lock= $$lock \n" ; \
		${CC} ./benchmark/tp-low-contention.c -o $(BUILD_DIR)/$@ -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		$(server) ./$(BUILD_DIR)/$@ $(threads) $(sampleSize) $(repetitions) $$lock ; \
	done 


bench: correctnessTest fairness latency tp-high-contention tp-low-contention

plotTPHigh:  
	python3 ./benchmark/plotTP.py high

plotTPLow: $(PLOTS_DIR)
	python3 ./benchmark/plotTP.py low


plotLatency: $(PLOTS_DIR)
	python3 ./benchmark/plotLatency.py

plotFairness: $(PLOTS_DIR)
	python3 ./benchmark/plotFairness.py


all-plots: plotTPHigh plotTPLow plotLatency plotFairness

small-plot: plotTPHigh plotTPLow plotLatency plotFairness

# Cleaning
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

clean-plots:
	rm -rf $(PLOTS_DIR)

clean-data: 
	rm -rf $(DATA_DIR)
