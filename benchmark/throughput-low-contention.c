#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <time.h>
#include <stdbool.h>
// #include "../locks/TestAndSetLock.c"
// #include "../locks/TestAndTestAndSetLocks.c"
#include "../locks/ArrayLock.c"
//#include "../locks/CLHLock.c" // We choose this lock
// #include "../locks/MCSLock.c"
// #include "../locks/HelmLock.c"



// Lock acquisition troughput benchmark
double benchmarkLockLowContention(int numberOfThreads, unsigned int sampleSize) {
    // Counter variable to track the total number of lock acquisitions
    int totalLockAcquisitions = 0;
    // Init shared number storage
    unsigned int sharedGenerator;
    // Init local number Storage 
    unsigned int randomSteps = 0;
    
    //
    // Declare Mutex and Init mutex
    // 

    Lock LOCK;
    init(&LOCK);


    // Start the timer
    double startTime = omp_get_wtime();

    // Perform the lock acquisition operation in a loop
    #pragma omp parallel for shared(LOCK ,totalLockAcquisitions)


        for (int j=0; j<sampleSize; j++){ // Loop over sampleSize 

            // 
            // Delay each thread randomly 
            //

            unsigned int localSeed = time(NULL) + omp_get_thread_num();
            srand(localSeed);
            unsigned int localSteps = rand_r(&localSeed) % 20;
            for (unsigned int j = 0; j < localSteps; ++j) {
                rand_r(&localSeed);
            }

            //
            // Aquire lock 
            // 

            lock(&LOCK); 
            //printf("Hej thread = %d aquired the lock \n ",omp_get_thread_num());

            //
            // Critical section
            // 
            //printf("Hej we got %d totalLockAquisitions \n ",totalLockAcquisitions);
            // Increment the total number of lock acquisitions
            // A short or empty critical section leads to high contention
            
            // Advance global random number generator in critical section
            for (unsigned int i = 0; i < 5; i++) {
                rand_r(&sharedGenerator);
            }

            totalLockAcquisitions++;
            //randomSteps += localSteps;

            //
            // Release the lock 
            // 

            unlock(&LOCK);
            //printf("Hej thread = %d has unlocked \n ",omp_get_thread_num());

            //
            // Increment the total number of lock acquisitions
            //
            //#pragma omp atomic
            //totalLockAcquisitions++;
        }
    

    //
    // Destroy the lock 
    // 
    
    destroy(&LOCK);

        
    // Calculate the elapsed time
    double endTime = omp_get_wtime();
    double elapsedSeconds = endTime - startTime;

    // Calculate the throughput
    //double throughput = ( totalLockAcquisitions * randomSteps) / elapsedSeconds;
    double throughput = (totalLockAcquisitions) / elapsedSeconds;

    // Print the results
    //printf("Total Lock Aquisitions: %d \n", totalLockAcquisitions);
    printf("Throughput: %.6f operations per second\n", throughput);

    return throughput;
}


void averageBench(int numThreads,unsigned int sampleSize){

    double averageThroughput = 0.0;
    unsigned int times = 5;

    for (size_t i=0; i<times; i++){
        averageThroughput += benchmarkLockLowContention(numThreads,sampleSize);
    }

    averageThroughput = averageThroughput/times;

    printf("Average Throughput: %.6f operations per second\n", averageThroughput); 
}


int main() {
    // Set the number of threads
    int numThreads[10] = {1,2,3,4,5,8,10,16,32,50};
    // Set sample Size
    unsigned int sampleSize = 1e4;
    printf("Lock Acquisitions:  %i \n", sampleSize);

    // Run the benchmark
    for(size_t i=0;i<10;i++){
        omp_set_num_threads(numThreads[i]);
        printf("Number of threads %i \n", numThreads[i]);
        averageBench(numThreads[i],sampleSize);
    }
    return 0;
}
