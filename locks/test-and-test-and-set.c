#include <omp.h>
#include <stdatomic.h>
#include <unistd.h>
#include <stdio.h>

atomic_bool lock_flag = ATOMIC_VAR_INIT(0);

void tas_lock() {
    while (1) {
        while (atomic_load_explicit(&lock_flag, memory_order_relaxed)) {}

        // attempt to acquire lock
        if (!atomic_exchange_explicit(&lock_flag, 1, memory_order_acquire)) {
            // lock acquired successfully
            return;
        }
    }
}

void tas_unlock() {
    atomic_store_explicit(&lock_flag, 0, memory_order_release);
}
