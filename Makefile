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
repetitions ?= 50
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
correctnessTest: ./src/correctnessTest.c  $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) IncorrectLock ; do \
		${CC} ./src/correctnessTest.c -o $(BUILD_DIR)/x -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		$(server) ./$(BUILD_DIR)/x $(threads) $(sampleSize) $$lock ; \
	done

fairness: src/fairness.c  $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		${CC} src/fairness.c -o build/x -fopenmp -include locks/$$lock.c ; \
		$(server) ./build/x $(threads) $(repetitions) $(sampleSize) $$lock; \
	done

latency: ./src/latency.c  $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		echo "Measuring Latency Lock= $$lock \n"  ; \
		${CC} ./src/latency.c -o $(BUILD_DIR)/$@ -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		$(server) ./$(BUILD_DIR)/$@ $(threads) $(sampleSize) $(repetitions) $$lock ; \
	done 

tp-high-contention: ./src/tp-high-contention.c $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		echo "Measuring throughput high contention. Lock= $$lock \n" ; \
		${CC} ./src/tp-high-contention.c -o $(BUILD_DIR)/$@ -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		$(server) ./$(BUILD_DIR)/$@ $(threads) $(sampleSize) $(repetitions) $$lock ; \
	done

tp-low-contention: ./src/tp-low-contention.c $(BUILD_DIR) $(DATA_DIR)
	@for lock in $(LOCKS) ; do \
		echo "Measuring throughput low contention. Lock= $$lock \n" ; \
		${CC} ./src/tp-low-contention.c -o $(BUILD_DIR)/$@ -fopenmp -include ./locks/$$lock.c ${CFLAGS} ; \
		$(server) ./$(BUILD_DIR)/$@ $(threads) $(sampleSize) $(repetitions) $$lock ; \
	done 


bench: correctnessTest fairness latency tp-high-contention tp-low-contention

plotTPHigh:  $(PLOTS_DIR)
	python3 ./src/plotTP.py high

plotTPLow: $(PLOTS_DIR)
	python3 ./src/plotTP.py low

plotLatency: $(PLOTS_DIR)
	python3 ./src/plotLatency.py

plotFairness: $(PLOTS_DIR)
	python3 ./src/plotFairness.py

small-plot: plotTPHigh plotTPLow plotLatency plotFairness


# Cleaning
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

clean-plots:
	rm -rf $(PLOTS_DIR)

clean-data: 
	rm -rf $(DATA_DIR)
