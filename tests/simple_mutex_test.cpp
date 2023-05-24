#include "../locks/test-and-set.cpp"

int main ()
{
    unsigned int microsecond = 1000000;

    std::cout << "pre lock: " << std::endl;

    #pragma omp parallel
    {
        std::cout << omp_get_thread_num() << std::endl;
        usleep(1 * microsecond);//sleeps for 1 second
        tas_lock();
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        std::cout << "in CS: " << omp_get_thread_num() << std::endl;
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        tas_unlock();
        std::cout << "unlocked: " << omp_get_thread_num() << std::endl;
    }
    return 0;
}