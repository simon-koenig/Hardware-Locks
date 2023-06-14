#include <stdio.h>
#include <omp.h>
#include "./src.c"


int main(int argc, char *argv[]) {
    // Set the number of threads
    // int numThreads = 16;
    // Set sample Size
    // int sampleSize = 1000;
    // Set repetitions
    // int reps = 5; 

    if (argc != 4) {
        printf("ERROR: Programm needs three input parameter: \n");
        printf("threads, sampleSize, repetitions \n");
        exit(EXIT_FAILURE);
    }

    int numThreads = atoi(argv[1]);
    int sampleSize = atoi(argv[2]);
    int reps = atoi(argv[3]);

    // Print sampleSize
    printf("Lock Acquisitions:  %i \n", sampleSize);
    // Run the benchmark
    omp_set_num_threads(numThreads);
    printf("Number of threads %i \n", numThreads);
    double latency = medianBenchLatency(sampleSize,reps);

    // Write to outfile
    char* filename = "Latency.txt";
    writeLatencyToFile(filename, sampleSize, reps, numThreads, latency);

    return 0;
}
