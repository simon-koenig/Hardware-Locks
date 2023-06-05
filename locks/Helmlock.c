#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct Helmlock_node{
   _Atomic struct Helmlock_Lock* Grant;
};

struct Helmlock_Lock{
   _Atomic struct Helmlock_node* mynode;
   _Atomic struct Helmlock_node* tail;
};

void Helmlock_init(struct Helmlock_Lock* self){
   atomic_store(&self->tail, (_Atomic struct Helmlock_node*) NULL); 
   atomic_store(&self->mynode, (_Atomic struct Helmlock_node*) NULL);
}

void Helmlock_lock(struct Helmlock_Lock* self){
    struct Helmlock_node* n = (struct Helmlock_node*)malloc(sizeof(struct Helmlock_node));
    atomic_store(&n->Grant, (_Atomic struct Helmlock_Lock*) NULL); 
    struct Helmlock_node* pred = (struct Helmlock_node*) atomic_exchange(&self->tail, (_Atomic struct Helmlock_node*) n);

    if (pred != (struct Helmlock_node*) NULL) {
        while (atomic_load(&pred->Grant) != (_Atomic struct Helmlock_Lock*) self);
        atomic_store(&pred->Grant, (_Atomic struct Helmlock_Lock*) NULL);
    }
    atomic_store(&self->mynode, (_Atomic struct Helmlock_node*) n);
}

void Helmlock_unlock(struct Helmlock_Lock* self)
{
    struct Helmlock_node* n = (struct Helmlock_node*) atomic_load(&self->mynode);
    atomic_store(&n->Grant, (_Atomic struct Helmlock_Lock*) NULL); 
    struct Helmlock_node* pred = (struct Helmlock_node*) atomic_load(&self->tail);

    if (pred != n){
        atomic_store(&n->Grant, (_Atomic struct Helmlock_Lock*) self);
        while(atomic_load(&n->Grant) != (_Atomic struct Helmlock_Lock*) NULL);
    }
    free(n);
}

void Helmlock_destroy_lock(struct Helmlock_Lock* self){
    free(self);
}

