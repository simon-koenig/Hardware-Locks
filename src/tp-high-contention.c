#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>
#include "./utils.c"


int main(int argc, char *argv[]) {
    // Set the number of threads
    int numThreads[11] = {1,2,3,4,5,8,10,16,32,50,64};

    if (argc != 5) {
        printf("ERROR: Programm needs four input parameter: \n");
        printf("threads, sampleSize, repetitions, lock \n");
        exit(EXIT_FAILURE);
    }

    // int dummyThreads = atoi(argv[1]);
    int sampleSize = atoi(argv[2]);
    int reps = atoi(argv[3]);

    // Print sampleSize
    printf("Lock Acquisitions:  %i \n", sampleSize);

    //
    // Run the benchmark
    // 

    Stats resultArray[11]; 
    for(size_t i=0;i<11;i++){
        omp_set_num_threads(numThreads[i]);
        resultArray[i] = dataBenchHighContention(sampleSize, reps);
    }  
    /*
    for(size_t i=0;i<11;i++){
        printf("Threads %i, values %f \n", numThreads[i],resultArray[i].average);  
    }   
    */
    // Write to outfile
    char *lockName = argv[4];
    char filename[100] = "TPHighContention";
    strcat(filename,lockName);
    strcat(filename,".txt");
 
    printf("Writing TP array to file ");
    writeThroughputArrayToFile(numThreads,resultArray, 11, filename, sampleSize, reps);

    return 0;
}
