# This is a comment line
CC=gcc
CFLAGS=-pedantic -Wall -Wextra -fsanitize=address -lm
LOCKS = BaselineLock TestAndSetLock TestAndTestAndSetLock TicketLock ArrayLock CLHLock MCSLock HemLock


latency: latency.c 
	@for lock in $(LOCKS) ; do \
		echo "Measuring Latency Lock= $$lock \n"  ; \
		${CC} ${CFLAGS} latency.c -o latency -fopenmp -include ../locks/$$lock.c ;\
		./$@ $(threads) $(sampleSize) $(repetitions) $$lock ;\
	done 


tp-high-contention: tp-high-contention.c
	@for lock in $(LOCKS) ; do \
		echo "Measuring throughput high contention. Lock= $$lock \n" ;\
		${CC} ${CFLAGS} $^ -o $@ -fopenmp -include ../locks/$$lock.c ;\
		./$@ $(threads) $(sampleSize) $(repetitions) $$lock ;\
	done
	

tp-low-contention: tp-low-contention.c 
	@for lock in $(LOCKS) ; do \
		echo "Measuring throughput low contention. Lock= $$lock \n" ; \
		${CC} ${CFLAGS} $^ -o $@ -fopenmp -include ../locks/$$lock.c ; \
		./$@ $(threads) $(sampleSize) $(repetitions) $$lock ; \
	done 


all: latency tp-high-contention tp-low-contention


clean:
	-rm *.txt
	-rm latency tp-high-contention tp-low-contention

