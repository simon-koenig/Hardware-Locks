#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


typedef omp_lock_t Lock;

void init(Lock* self){
    omp_init_lock(self);
}


void lock(Lock* self) {
    omp_set_lock(self);
}

void unlock(Lock* self) {
    omp_unset_lock(self);
}

void destroy(Lock* self){
   omp_destroy_lock(self);
}