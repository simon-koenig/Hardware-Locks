#include <stdio.h>
#include <omp.h>
#include "../locks/array-lock.c"


// Lock acquisition troughput benchmark
double benchmarkLockHighContention(int numberOfThreads, unsigned int sampleSize) {
    // Barrier to force OMP to start all threads at the same time
    #pragma omp barrier
    // Counter variable to track the total number of lock acquisitions
    int totalLockAcquisitions = 0;

    // Start the timer
    double startTime = omp_get_wtime();

    // Perform the lock acquisition operation in a loop
    #pragma omp parallel for shared(totalLockAcquisitions)
    
        // Each thread performs the lock acquisition operation
        for (int i=0; i<numberOfThreads; i++) {
            // A sampleSize number of times  
            for (int j=0; j<sampleSize; j++){

                //
                // Aquire lock 
                // 
                lock();
                //printf("Hej thread = %d aquired the lock \n ",omp_get_thread_num());

                //
                // Critical section
                // 
                //printf("Hej we got %d totalLockAquisitions \n ",totalLockAcquisitions);
                // Increment the total number of lock acquisitions
                // A short or empty critical section leads to high contention
                totalLockAcquisitions++;

                //
                // Release the lock 
                // 
                unlock();
                //printf("Hej thread = %d has unlocked \n ",omp_get_thread_num());

                //
                // Increment the total number of lock acquisitions
                //
                //#pragma omp atomic
                //totalLockAcquisitions++;
            }
        }

    // Calculate the elapsed time
    double endTime = omp_get_wtime();
    double elapsedSeconds = endTime - startTime;

    // Calculate the throughput
    double throughput = totalLockAcquisitions / elapsedSeconds;

    // Print the results
    //printf("Total Lock Aquisitions: %d \n", totalLockAcquisitions);
    //printf("Throughput: %.6f operations per second\n", throughput);

    return throughput;
}


void averageBench(int numThreads,unsigned int sampleSize){

    double averageThroughput = 0.0;
    unsigned int times = 5;
    for (size_t i=0; i<times; i++){
        averageThroughput += benchmarkLockHighContention(numThreads,sampleSize);
    }
    averageThroughput = averageThroughput/times;

    printf("Average Throughput: %.6f operations per second\n", averageThroughput); 
    
}


int main() {
    // Set the number of threads
    int numThreads[8] = {1,2,4,8,16,32,50,100};
    // Set sample Size
    unsigned int sampleSize = 20;

    // Run the benchmark
    for(size_t i=0;i<8;i++){
        omp_set_num_threads(numThreads[i]);
        printf("Number of threads %i \n", numThreads[i]);
        averageBench(numThreads[i],sampleSize);

    }

    return 0;
}
