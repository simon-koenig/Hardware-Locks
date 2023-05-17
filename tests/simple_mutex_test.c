#include "../locks/test-and-set.c"

int main ()
{
    unsigned int microsecond = 1000000;

    printf("pre lock:");

    #pragma omp parallel
    {
        printf("%d \n", omp_get_thread_num());
        usleep(1 * microsecond);//sleeps for 1 second
        tas_lock();
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        printf("in lock: %d \n", omp_get_thread_num());;
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        tas_unlock();
        printf("unlocked: %d \n", omp_get_thread_num());;
    }
    return 0;
}