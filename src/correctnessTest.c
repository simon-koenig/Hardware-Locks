#include <string.h>

int main(int argc, char *argv[]) {

    int thread_number = atoi(argv[1]);
    omp_set_num_threads(thread_number);
    int array_size = atoi(argv[2]);
    char* lockp = argv[3];

    // get lock as char array
    char lockname[strlen(lockp) + 1];
    strcpy(lockname, lockp);

    // define filename
    char filename[60] ="data/correctness/";
    strcat(filename, lockname);
    strcat(filename, ".txt");

    int counter = 0;

    float array_sum_seqential = 0, array_sum_parallel = 0;

    // generate random array + sum it up sequentially
    float array[array_size];
    for (int i = 0; i < array_size; i++){
        array[i] = (float)rand() / (float)RAND_MAX;
        array_sum_seqential = array_sum_seqential + array[i];
    }

    printf("\n%s:\n", lockname);
    printf("thread number: %d, array size: %d\n", thread_number, array_size);
    printf("-----------------------------------\n");

    Lock LOCK;
    init(&LOCK);
    
    #pragma omp parallel for shared(LOCK, counter)

    // sum parallel
    for(int i=0; i < array_size; i++){

        // Acquire the lock
        lock(&LOCK);

        // Add elementwise
        array_sum_parallel += array[counter];

        // Thread is in critical section - write to counter 
        counter += 1; 

        // Release the lock 
        unlock(&LOCK);
        }

    destroy(&LOCK);

    // write results to file
    FILE *fp = fopen(filename, "w");

    fprintf(fp, "Lock name: %s\n", lockname);
    fprintf(fp, "Thread number: %d\n", thread_number);
    fprintf(fp, "Array size: %d\n", array_size);
    fprintf(fp, "--------------------------------------------\n");
    fprintf(fp, "Sequential sum:\t%.4f\nParallel sum:\t%.4f\n", array_sum_seqential, array_sum_parallel);
    fprintf(fp, "--------------------------------------------\n");
    if (array_sum_seqential != array_sum_parallel){
        fprintf(fp, "Sums of arrays not equal - correctness violated!!!\n");
        printf("Sums of arrays not equal - correctness violated!!!\n");
    } else {
        fprintf(fp, "No violations in correctness found.\n");
        printf("No violations in correctness found.\n");
    }

    

    return 0;
}
