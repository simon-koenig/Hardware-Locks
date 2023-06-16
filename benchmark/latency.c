#include <stdio.h>
#include <omp.h>
#include <string.h>
#include "./src.c"


int main(int argc, char *argv[]) {
    // Set the number of threads
    // int numThreads = 16;
    // Set sample Size
    // int sampleSize = 1000;
    // Set repetitions
    // int reps = 5; 

    if (argc != 5) {
        printf("ERROR: Programm needs four input parameter: \n");
        printf("threads, sampleSize, repetitions, lock \n");
        exit(EXIT_FAILURE);
    }

    int numThreads = atoi(argv[1]);
    int sampleSize = atoi(argv[2]);
    int reps = atoi(argv[3]);

    // Print sampleSize
    // printf("Lock Acquisitions:  %i \n", sampleSize);
    // Run the benchmark
    omp_set_num_threads(numThreads);
    // printf("Number of threads %i \n", numThreads);
    Stats latencyData = dataBenchLatency(sampleSize,reps);

    // Write to outfile
    char *lockName = argv[4];
    char filename[100] = "Latency";
    strcat(filename,lockName);
    strcat(filename,".txt");

    writeLatencyToFile(filename, sampleSize, reps, numThreads, latencyData);

    return 0;
}
