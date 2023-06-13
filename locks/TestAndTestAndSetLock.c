#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Lock {
atomic_bool lock_flag;
} Lock;

void init(Lock* self){
    atomic_store_explicit(&self->lock_flag, 0, memory_order_release);
}

void lock(Lock* self) {
    while (1) {
        while (atomic_load_explicit(&self->lock_flag, memory_order_relaxed)) {}

        // attempt to acquire lock
        if (!atomic_exchange_explicit(&self->lock_flag, 1, memory_order_acquire)) {
            // lock acquired successfully
            return;
        }
    }
}

void unlock(Lock* self) {
    atomic_store_explicit(&self->lock_flag, 0, memory_order_release);
}

void destroy(Lock* self){
    atomic_store_explicit(&self->lock_flag, 0, memory_order_release);
}
