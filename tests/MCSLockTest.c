#include "../locks/MCSLock.c"

int main ()
{
    unsigned int microsecond = 1000000;
    omp_set_num_threads(8); // Has to be <= than the number of flags defined in the array
    Lock LOCK;
    init(&LOCK);

    #pragma omp parallel shared(LOCK)
    {   /* Only master thread does this */
        if (omp_get_thread_num()==0) {
            printf("Number of threads = %d\n", omp_get_num_threads());
            }
       
        usleep(1 * microsecond); //sleeps for 1 second
        // Acquire the lock
        lock(&LOCK);
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        printf("Hej thread = %d aquired the lock \n ",omp_get_thread_num());
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        // Release the lock 
        unlock(&LOCK);
        printf("Hej thread = %d has unlocked \n ",omp_get_thread_num());

    }
    
    // Free the lock
    destroy(&LOCK);
    return 0;
}
