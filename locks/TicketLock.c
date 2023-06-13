#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Lock {
    atomic_int next_ticket;
    atomic_int now_serving;
} Lock;

void init(Lock* self) {
    self->next_ticket = 0;
    self->now_serving = 0;
}

void lock (Lock* self) {
    int my_ticket = atomic_fetch_add(&self->next_ticket, 1);
    while (atomic_load(&self->now_serving) != my_ticket); // Spin until it's my turn
}

void unlock(Lock* self) {
    atomic_fetch_add(&self->now_serving, 1);
}

void destroy(Lock* self) {
    // No special cleanup required for a ticket Lock
}
/*
// Example usage
int main() {
    Lock my_Lock;
    init(&my_Lock);

    // Critical section
    Lock(&my_Lock);
    printf("Inside the critical section.\n");
    unLock(&my_Lock);

    destroy(&my_Lock);

    return 0;
}
*/