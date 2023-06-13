int main(int argc, char *argv[]) {

    int thread_number = atoi(argv[1]);
    omp_set_num_threads(thread_number);
    int array_size = atoi(argv[2]);

    _Atomic int lock_calls = 0;
    int counter = 0;

    float array_sum_seqential = 0, array_sum_parallel = 0;

    // generate random array + sum it up sequentially
    float array[array_size];
    for (int i = 0; i < array_size; i++){
        array[i] = (float)rand() / (float)RAND_MAX;
        array_sum_seqential = array_sum_seqential + array[i];
    }

    Lock LOCK;
    init(&LOCK);

    printf("Hello from threads ...\n");
    
    #pragma omp parallel shared(LOCK, counter, lock_calls)
    {

        printf("%d \n", omp_get_thread_num());
        
        // sum parallel
        while(atomic_load(&lock_calls) < array_size){

            // add 1 to lock_calls
            atomic_fetch_add(&lock_calls, 1);

            // Acquire the lock
            lock(&LOCK);

            // Add elementwise
            array_sum_parallel += array[counter];

            // Thread is in critical section - write to counter 
            counter += 1; 

            // Release the lock 
            unlock(&LOCK);
           }
    }

    destroy(&LOCK);

    printf("\nArraysize: %d\n", array_size);
    printf("--------------------------------------------\n");
    printf("Sequential sum:\t%.4f\nParallel sum:\t%.4f\n", array_sum_seqential, array_sum_parallel);
    printf("--------------------------------------------\n");
    if (array_sum_seqential != array_sum_parallel){
        printf("ERROR: sums of arrays not equal - correctness violated!\n");
    } else {
        printf("No violations in correctness found!\n");
    }

    return 0;
}
