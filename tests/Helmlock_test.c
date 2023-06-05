#include "../locks/Helmlock.c"

int main ()
{
    unsigned int microsecond = 1000000;
    omp_set_num_threads(8);

    // Declare Mutex
    struct Helmlock_Lock HELMLOCK;
    // Initialize Mutex
    Helmlock_init(&HELMLOCK);

    printf("Before Lock\n");
    #pragma omp parallel shared(HELMLOCK)
    {   /* Only master thread does this */
        if (omp_get_thread_num()==0) {
            printf("Number of threads = %d\n", omp_get_num_threads());
            }
        usleep(1 * microsecond); //sleeps for 1 second
        // Acquire the lock
        Helmlock_lock(&HELMLOCK);
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        printf("Hej thread = %d aquired the lock \n ",omp_get_thread_num());
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        // Release the lock 
        Helmlock_unlock(&HELMLOCK);
        printf("Hej thread = %d has unlocked \n ",omp_get_thread_num());
   
    }
    printf("After Lock\n");
    return 0;

}