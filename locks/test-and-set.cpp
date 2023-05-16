#include <omp.h>
#include <iostream>
#include <atomic>
#include <unistd.h>

std::atomic_flag locked = ATOMIC_FLAG_INIT; // atomic register

void lock() {
    while (locked.test_and_set()){}
    return;
}
void unlock () {
    locked.clear();
    std::cout << "unlocked: " << omp_get_thread_num() << std::endl;
}

int main ()
{
    std::cout << "pre lock: " << std::endl;
    unsigned int microsecond = 1000000;
    #pragma omp parallel
    {
        std::cout << omp_get_thread_num() << std::endl;
        usleep(1 * microsecond);//sleeps for 1 second
        lock();
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        std::cout << "in lock: " << omp_get_thread_num() << std::endl;
        usleep(0.5 * microsecond);//sleeps for 0.5 second
        unlock();
    }
    return 0;
}
