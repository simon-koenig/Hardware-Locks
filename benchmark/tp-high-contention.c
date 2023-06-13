#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
// #include "../locks/TestAndSetLock.c"
// #include "../locks/TestAndTestAndSetLock.c"
//#include "../locks/ArrayLock.c"
// #include "../locks/CLHLock.c" // We choose this lock
// #include "../locks/MCSLock.c"
// #include "../locks/HelmLock.c"
#include "../locks/Baseline.c"
#include "./src"


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
        medianBenchHighContention(numThreads[i],sampleSize);
    }
    return 0;
}
