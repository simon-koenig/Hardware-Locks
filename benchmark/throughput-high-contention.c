#include <stdio.h>
#include <omp.h>
// #include "../locks/TestAndSetLock.c"
// #include "../locks/TestAndTestAndSetLock.c"
#include "../locks/ArrayLock.c"
// #include "../locks/CLHLock.c" // We choose this lock
// #include "../locks/MCSLock.c"
// #include "../locks/HelmLock.c"

// Lock acquisition troughput benchmark
double benchmarkLockHighContention(int numberOfThreads, unsigned int sampleSize) {
    // Barrier to force OMP to start all threads at the same time
    #pragma omp barrier
    // Counter variable to track the total number of lock acquisitions
    int totalLockAcquisitions = 0;

    //
    // Declare Mutex and Init mutex
    // 
    Lock LOCK;
    init(&LOCK);


    // Start the timer
    double startTime = omp_get_wtime();

    // Perform the lock acquisition operation in a loop
    #pragma omp parallel for shared(LOCK, totalLockAcquisitions)
    
        // Each thread performs the lock acquisition operation
        // A sampleSize number of times  
        for (int j=0; j<sampleSize; j++){

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
            totalLockAcquisitions++;

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
        
    

    // Calculate the elapsed time
    double endTime = omp_get_wtime();
    double elapsedSeconds = endTime - startTime;

    // Calculate the throughput
    double throughput = totalLockAcquisitions / elapsedSeconds;

    // Print the results
    //printf("Total Lock Aquisitions: %d \n", totalLockAcquisitions);
    //printf("Throughput: %.6f operations per second\n", throughput);

    //
    // Destroy the lock 
    // 
    
    destroy(&LOCK);

    return throughput;
}


void medianBench(int numThreads,unsigned int sampleSize){

     unsigned int times = 5;
    double throughputMeasurements[times];

    for (size_t i=0; i<times; i++){
        throughputMeasurements[i] = benchmarkLockLowContention(numThreads,sampleSize);
        printf("Throughput: %.6f operations per second\n", throughputMeasurements[i]); 
    }

    qsort(throughputMeasurements,times ,sizeof(double), compare);  
    double medianThroughput = throughputMeasurements[times/2];

    printf("Median Throughput: %.6f operations per second\n", medianThroughput); 
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
        medianBench(numThreads[i],sampleSize);
    }
    return 0;
}
