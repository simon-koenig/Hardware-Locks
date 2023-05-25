#include "../locks/array-lock.c"

int main ()
{
    unsigned int microsecond = 1000000;
    omp_set_num_threads(8); // Has to be <= than the number of flags defined in the array


    printf("Before Lock");
    #pragma omp parallel
    {   /* Only master thread does this */
        if (omp_get_thread_num()==0) {
            printf("Number of threads = %d\n", omp_get_num_threads());
            }
        
        // printf("Hej from thread = %d\n",omp_get_thread_num());
        usleep(1 * microsecond);//sleeps for 1 second
        lock();
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        printf("Hej thread = %d aquired the lock \n ",omp_get_thread_num());
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        unlock();
        printf("Hej thread = %d has unlocked \n ",omp_get_thread_num());
    }
    printf("After Lock\n");
    return 0;
}