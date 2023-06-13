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
    while (atomic_exchange_explicit(&self->lock_flag, 1, memory_order_acquire)){};
}

void unlock(Lock* self) {
    atomic_store_explicit(&self->lock_flag, 0, memory_order_release);
}

void destroy(Lock* self){
    atomic_store_explicit(&self->lock_flag, 0, memory_order_release);
}
