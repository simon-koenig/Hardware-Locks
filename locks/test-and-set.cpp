#include <omp.h>
#include <iostream>
#include <atomic>
#include <unistd.h>

std::atomic_flag locked = ATOMIC_FLAG_INIT; // atomic register

void tas_lock() {
    while (locked.test_and_set()){}
    return;
}
void tas_unlock () {
    locked.clear();
}


