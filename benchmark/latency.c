#include <stdio.h>
#include <omp.h>
// #include "../locks/TestAndSetLock.c"
// #include "../locks/TestAndTestAndSetLocks.c"
// #include "../locks/ArrayLock.c"
#include "../locks/CLHLock.c" // We choose this lock
// #include "../locks/MCSLock.c"
// #include "../locks/HelmLock.c"



// Lock acquisition troughput benchmark
double benchmarkLockLatency(int numberOfThreads, unsigned int sampleSize) {
    // Barrier to force OMP to start all threads at the same time
    #pragma omp barrier

    double elapsedSeconds = 0; // Total elapsed Seconds for lock aquisition 
    double longestWait = 0; 

    //
    // Declare Mutex and Init mutex
    // 

    Lock LOCK;
    init(&LOCK);



    // Perform the lock acquisition operation in a loop
    #pragma omp parallel for
    
    // Each thread performs the lock acquisition operation
        // A sampleSize number of times  
        for (int j=0; j<sampleSize; j++){
            //
            // Aquire lock and time 
            // 
            double startTime = omp_get_wtime(); // Start the timer
            lock(&LOCK);
            double endTime = omp_get_wtime(); // End timer 

            double waitTime = endTime - startTime; 

            elapsedSeconds += waitTime; // Add to the the elapsed time

            if (waitTime > longestWait){ longestWait = waitTime;}; // Update longest Wait if necessary 


            // Critical section is empty since we just measure lock aquisition latency

            //
            // Release the lock 
            // 
            unlock(&LOCK);
            
        }

    //
    // Destroy the lock 
    // 
    
    destroy(&LOCK);


    // Calculate average latency 
    double latency = elapsedSeconds/( sampleSize )  ; 

    // Print the results
    //printf("Total Lock Aquisitions: %d \n", totalLockAcquisitions);
    printf("Longest Wait : %.10f ns\n", longestWait*1e9);

    return latency;
}


void averageBench(int numThreads,unsigned int sampleSize){

    double averageLatency = 0.0;
    unsigned int times = 7;
    for (size_t i=0; i<times; i++){
        averageLatency += benchmarkLockLatency(numThreads,sampleSize);
    }
    averageLatency = averageLatency/times;

    printf("Average Latency: %.10f ns per lock aquisition\n", averageLatency*1e9);  
}

int main() {
    // Set the number of threads
    int numThreads = 16;
    // Set sample Size
    unsigned int sampleSize = 1000;

    // Run the benchmark
    omp_set_num_threads(numThreads);
    printf("Number of threads %i \n", numThreads);
    averageBench(numThreads,sampleSize);
    
    return 0;
}
