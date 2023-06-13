#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Lock {
    atomic_int ticket;
    atomic_int served;
} Lock;

void init(Lock* self) {
    self->ticket = 0;
    self->served = 0;
}

void lock (Lock* self) {
    int my_ticket = atomic_fetch_add(&self->ticket, 1);
    while (atomic_load(&self->served) != my_ticket); // Spin until it's my turn
}

void unlock(Lock* self) {
    atomic_fetch_add(&self->served, 1);
}

void destroy(Lock* self) {
    self->ticket = 0;
    self->served = 0;
}