#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>


// 
// Source code
// 

// Struct to save average and standard Deviation
typedef struct {
    double average;
    double stdDeviation;
} Stats;

double standardDeviation(double array[],double mean, int times){
    double squaredSum = 0.0; 
    for (int i = 0; i < times; i++) {
        double diff = array[i] - mean;
        squaredSum += diff * diff;
    }

    double stdDev = sqrt(squaredSum/times);
    return stdDev;

}


// Low Contention Benchmark
double benchmarkLockLowContention(int sampleSize) {
    // Counter variable to track the total number of lock acquisitions
    int totalLockAcquisitions = 0;
    // Init shared number storage
    unsigned int sharedGenerator;

    
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
            for (int i = 0; i < 5; i++) {
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
    double throughput = (totalLockAcquisitions * 5) / elapsedSeconds;

    // Print the results
    //printf("Total Lock Aquisitions: %d \n", totalLockAcquisitions);
    //printf("Throughput: %.6f operations per second\n", throughput);

    return throughput;
}

Stats dataBenchLowContention(int sampleSize, int times){
    // Init data struct
    Stats data;
    double throughputMeasurements[times];
    double sum; 
    for (int i=0; i<times; i++){
        double value = benchmarkLockLowContention(sampleSize);
        throughputMeasurements[i] = value;
        sum += value;
        //printf("Throughput: %.6f operations per second\n", throughputMeasurements[i]); 
    }

    //qsort(throughputMeasurements,times ,sizeof(double), compareDoubles);  
   // double medianThroughput = throughputMeasurements[times/2];

    // printf("Median Throughput: %.6f operations per second\n", medianThroughput); 

    // Calc average value
    data.average = sum / times;

    // Calc standard deviation
    data.stdDeviation = standardDeviation(throughputMeasurements,data.average,times);

    return data; 
}

// High Contention Benchmark 
double benchmarkLockHighContention(int sampleSize) {
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
    double throughput = ( totalLockAcquisitions + 5 ) / elapsedSeconds;

    // Print the results
    //printf("Total Lock Aquisitions: %d \n", totalLockAcquisitions);
    //printf("Throughput: %.6f operations per second\n", throughput);

    //
    // Destroy the lock 
    // 
    
    destroy(&LOCK);

    return throughput;
}

Stats dataBenchHighContention(int sampleSize,int times){
    // Init data struct
    Stats data;
    double throughputMeasurements[times];
    double sum; 
    for (int i=0; i<times; i++){
        double value = benchmarkLockHighContention(sampleSize);
        throughputMeasurements[i] = value;
        sum += value;
        //printf("Throughput: %.6f operations per second\n", throughputMeasurements[i]); 
    }

    //qsort(throughputMeasurements,times ,sizeof(double), compareDoubles);  
   // double medianThroughput = throughputMeasurements[times/2];

    // printf("Median Throughput: %.6f operations per second\n", medianThroughput); 

    // Calc average value
    data.average = sum / times;

    // Calc standard deviation
    data.stdDeviation = standardDeviation(throughputMeasurements,data.average,times);

    return data; 
}

// Latency Benchmark
double benchmarkLockLatency(int sampleSize) {
    // Barrier to force OMP to start all threads at the same time
    #pragma omp barrier

    double elapsedSeconds = 0; // Total elapsed Seconds for lock aquisition 
    //double longestWait = 0; 

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

            //if (waitTime > longestWait){ longestWait = waitTime;}; // Update longest Wait if necessary 

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

    return latency;
}

// Latency Benchmark
Stats dataBenchLatency(int sampleSize, int times){
    // Init data struct
    Stats data;
    double LatencyMeasurements[times];
    double sum;

    for (int i=0; i<times; i++){
        double value = benchmarkLockLatency(sampleSize);
        LatencyMeasurements[i] = value;
        sum += value;
       // printf("Throughput: %.6f operations per second\n", throughputMeasurements[i]); 
    }

    //qsort(LatencyMeasurements,times ,sizeof(double), compareDoubles);  
    //double medianLatency = LatencyMeasurements[times/2];

    //printf("Median Latency: %.6f us \n", medianLatency * 1e6); 

    // Calc average value
    data.average = sum / times;


    // Calc standard deviation
    data.stdDeviation = standardDeviation(LatencyMeasurements,data.average,times);


    return data; 
}

// Helper function for output to text files
void writeThroughputArrayToFile(int threads[], Stats tp[], int N, char* filename, int sampleSize, int repetitions)
{   
    char datafile[100] = "./data/throughput/";
    strcat(datafile,filename);
    FILE *fp = fopen(datafile, "w");

    // store basic information about run
    fprintf(fp, "--------------------- \n");
    fprintf(fp, "# sampleSize, %i \n", sampleSize);
    fprintf(fp, "# repetitions, %i \n", repetitions);
    fprintf(fp, "--------------------- \n");
    fprintf(fp, "numberOfThreads, Throughput [ops/s], standardDeviation [ops/s] \n");    

    // store array
    for(int i = 0; i<N; i++){
        fprintf(fp, "%i, %f, %f \n", threads[i] ,tp[i].average, tp[i].stdDeviation);
    }

    fclose(fp);
}

void writeLatencyToFile(char* filename, int sampleSize, int repetitions, int numberOfThreads, Stats latency)
{   
    char datafile[120] = "./data/latency/";
    strcat(datafile,filename);
    FILE *fp = fopen(datafile, "w");

    // store basic information about run
    fprintf(fp, "--------------------- \n");
    fprintf(fp, "# sampleSize, %i \n", sampleSize);
    fprintf(fp, "# repetitions, %i \n", repetitions);
    fprintf(fp, "# numberOfThreads, %i \n",numberOfThreads );
    fprintf(fp, "--------------------- \n");
    fprintf(fp, "Latency [ns], standardDeviation [ns] \n");    

    // store value
    fprintf(fp, "%f,  %f\n", latency.average*1e9, latency.stdDeviation*1e9);
    
    
    fclose(fp);
}