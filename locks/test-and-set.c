#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>

atomic_bool lock_flag = ATOMIC_VAR_INIT(0);

void tas_lock() {
    while (atomic_exchange_explicit(&lock_flag, 1, memory_order_acquire)){};
}

void tas_unlock() {
    atomic_store_explicit(&lock_flag, 0, memory_order_release);
}
