#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Lock {
  bool* flags;
  // Making an atomic counter
  _Atomic int tail;
} Lock;

// Thread local variable 
__thread int mySlot;



void init(Lock* self){
    self->flags = malloc(sizeof(bool) * omp_get_max_threads());
    for (int i=1; i<omp_get_max_threads(); i++) { self->flags[i] = false; }
    //self->flags[0] = true;
    atomic_store_explicit(&self->flags[0], true, memory_order_relaxed);
    atomic_store(&self->tail, 0);   
}


void lock(Lock* self) {
    mySlot = atomic_fetch_add(&self->tail, 1) % omp_get_max_threads();
    
    while (!self->flags[mySlot]) {
        // Wait until the flag at mySlot is set to true
    }
}

void unlock(Lock* self) {
    self->flags[mySlot] = false;
    self->flags[(mySlot + 1) % omp_get_max_threads()] = true;
}


void destroy(Lock* self){
    free(self->flags);
}

