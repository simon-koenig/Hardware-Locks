#include "../locks/MCS.c"

int main ()
{
    unsigned int microsecond = 1000000;
    omp_set_num_threads(8);

    // Declare Mutex
    struct MCS_Lock MCS_LOCK;
    // Initialize Mutex
    MCS_init(&MCS_LOCK);

    printf("Before Lock\n");
    #pragma omp parallel shared(MCS_LOCK)
    {   /* Only master thread does this */
        if (omp_get_thread_num()==0) {
            printf("Number of threads = %d\n", omp_get_num_threads());
            }
        usleep(1 * microsecond); //sleeps for 1 second
        // Acquire the lock
        MCS_lock(&MCS_LOCK);
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        printf("Hej thread = %d aquired the lock \n ",omp_get_thread_num());
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        // Release the lock 
        MCS_unlock(&MCS_LOCK);
        printf("Hej thread = %d has unlocked \n ",omp_get_thread_num());
   
    }
    MCS_destroy_lock(&MCS_LOCK);
    printf("After Lock\n");
    return 0;

}