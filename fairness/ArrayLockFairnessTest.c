#include "../locks/ArrayLock.c"

#include<stdio.h>
#include <string.h>

void writeArrayToFile(double* p, int N, char* filename, int threads, int max_iter, int repetitions)
{
    FILE *fp = fopen(filename, "w");

    // store basic information about run
    fprintf(fp, "--------------------- \n");
    fprintf(fp, "# threads, %i \n", threads);
    fprintf(fp, "# lock-calls, %i \n", max_iter);
    fprintf(fp, "# repetitions, %i \n", repetitions);
    fprintf(fp, "--------------------- \n");
    fprintf(fp, "thread ID, # CS reached \n");    

    // store array
    for(int i = 0; i<N; i++){
        fprintf(fp, "%i, %f \n", i ,p[i]);
    }
    
    fclose(fp);
}


int main (int argc, char *argv[]){

    if (argc != 4) {
        printf("ERROR: Programm needs three input parameter: \n");
        printf("number of threads, number of lock calls, repetitions \n");
        exit(EXIT_FAILURE);
    }

    int thread_number = atoi(argv[1]);
    omp_set_num_threads(thread_number);
    int max_lock_calls = atoi(argv[2]);
    int reps = atoi(argv[3]);

    // Declare Mutex
    Lock LOCK;
    // Initialize Mutex
    init(&LOCK);

    // number of lock calls
    _Atomic int lock_calls = 0;

    // set up a counter-array
    double* counter;
    counter = malloc(sizeof(double)*omp_get_max_threads());
    for (int i=0; i<omp_get_max_threads(); i++) { counter[i] = 0; }   


    for (int iter = 0; iter < reps; iter++){

        #pragma omp parallel shared(LOCK, counter, lock_calls)
        {  
            while(atomic_load(&lock_calls) < max_lock_calls){


            // add 1 to lock_calls
            atomic_fetch_add(&lock_calls, 1);

            // Acquire the lock
            lock(&LOCK);

            // Thread is in critical section - write to counter array 
            counter[omp_get_thread_num()] += 1.0;       

            // Release the lock 
            unlock(&LOCK);
           }     
        }
        
        // reset lock_calls
        atomic_store(&lock_calls, 0); 
    }

    // Calculate average of all repetitions
    for (int i=0; i<omp_get_max_threads(); i++) { counter[i] = counter[i]/reps; }   


    char* filename = "ArrayLockFairnessTestResults.txt";
 

    writeArrayToFile(counter, omp_get_max_threads(), filename, thread_number, max_lock_calls, reps);

    // Free the lock
    destroy(&LOCK);

    free(counter);
    return 0;
}