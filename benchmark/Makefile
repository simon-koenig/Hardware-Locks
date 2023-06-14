# This is a comment line
CC=gcc
CFLAGS=-pedantic -Wall -Wextra -fsanitize=address

latency: latency.c 
	@echo "Measuring Latency Lock =  $(lock):\n"
	${CC} ${CFLAGS} latency.c -o latency -fopenmp -include ../locks/$(lock).c
	./$@ $(threads) $(sampleSize) $(repetitions)
	

tp-high-contention: tp-high-contention.c 
	@echo "Measuring throughput high contention. Lock =  $(lock):\n"
	${CC} ${CFLAGS} $^ -o $@ -fopenmp -include ../locks/$(lock).c
	./$@ $(threads) $(sampleSize) $(repetitions)
	

tp-low-contention: tp-low-contention.c 
	@echo "Measuring throughput low contention. Lock =  $(lock):\n"
	${CC} ${CFLAGS} $^ -o $@ -fopenmp -include ../locks/$(lock).c
	./$@ $(threads) $(sampleSize) $(repetitions)

all: latency tp-high-contention tp-low-contention

clean:
	rm latency tp-high-contention tp-low-contention
