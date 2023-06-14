#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <stdint.h>
#include <strings.h>
#include <time.h>
#include <stdbool.h>
#include "./src.c"


int main(int argc, char *argv[]) {
    // Set the number of threads
    int numThreads[10] = {1,2,3,4,5,8,10,16,32,50};

    if (argc != 4) {
        printf("ERROR: Programm needs three input parameter: \n");
        printf("threads, sampleSize, repetitions \n");
        exit(EXIT_FAILURE);
    }

    //
    // int dummyThreads = atoi(argv[1]);
    int sampleSize = atoi(argv[2]);
    int reps = atoi(argv[3]);

    // Print sampleSize
    printf("Lock Acquisitions:  %i \n", sampleSize);

    //
    // Run the benchmark
    // 

    double resultArray[10]; 
    for(size_t i=0;i<10;i++){
        omp_set_num_threads(numThreads[i]);
        printf("Number of threads %i \n", numThreads[i]);
        resultArray[i] = medianBenchLowContention(sampleSize, reps);
    }


    char* filename = "TPLowContention.txt";
 

    writeThroughputArrayToFile(numThreads,resultArray, 10, filename, sampleSize, reps);

    return 0;
}