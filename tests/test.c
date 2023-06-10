#pragma once

// execute with clang -o x test.c -fopenmp -include ../locks/CLHLock.c

#include <stdlib.h>

#define ARRAY_SIZE 8

int main() {

    unsigned int microsecond = 1000;
    omp_set_num_threads(8);

    Lock LOCK;
    init(&LOCK);

    int counter = 0;
    float array_sum_seqential = 0, array_sum_parallel = 0;

    // generate random array + sum it up sequentially
    float array[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = (float)rand() / RAND_MAX;
        array_sum_seqential = array_sum_seqential + array[i];
    }

    printf("Hello from threads ...\n");
    
    #pragma omp parallel shared(counter)
    {

        printf("%d \n", omp_get_thread_num());
        
        // sum parallel
        //while(counter < 1 * ARRAY_SIZE){
            lock(&LOCK);
            //printf("\n%d", omp_get_thread_num());
            array_sum_parallel = array_sum_parallel + array[counter];
            usleep(10 * microsecond);
            //printf("\t%d", counter);
            counter = counter + 1;
            unlock(&LOCK);
        //}
    }

    destroy(&LOCK);

    printf("\nSequential sum:\t%.4f\nParallel sum:\t%.4f\n", array_sum_seqential, array_sum_parallel);

    if (array_sum_seqential != array_sum_parallel){
        printf("ERROR: sums of arrays not equal -> correctness violated\n");
    } else {
        printf("No violations in correctness found\n");
    }

    return 0;
}
