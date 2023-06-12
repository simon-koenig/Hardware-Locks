#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node{
   _Atomic struct Lock* Grant;
} node;

typedef struct Lock{
   _Atomic struct node* tail;
} Lock;

__thread struct node mynode = {(_Atomic struct Lock*) NULL};

void init(struct Lock* self){
   atomic_store(&self->tail, (_Atomic struct node*) NULL); 
}

void lock(struct Lock* self){
    struct node* n = &mynode;
    atomic_store(&n->Grant, (_Atomic struct Lock*) NULL); 
    struct node* pred = (struct node*) atomic_exchange(&self->tail, (_Atomic struct node*) n);

    if (pred != (struct node*) NULL) {
        while (__sync_val_compare_and_swap(&pred->Grant, self, (_Atomic struct Lock*) NULL) != (_Atomic struct Lock*) self) {};
        atomic_store(&pred->Grant, (_Atomic struct Lock*) NULL);
    }
}

void unlock(struct Lock* self)
{
    struct node* n = &mynode;
    //atomic_store(&n->Grant, (_Atomic struct Lock*) NULL);
    struct node* pred = (struct node*) __sync_val_compare_and_swap(&self->tail, (_Atomic struct node*) n, (_Atomic struct node*) NULL);

    if (pred != n){
        atomic_store(&n->Grant, (_Atomic struct Lock*) self);
        while(atomic_load(&n->Grant) != (_Atomic struct Lock*) NULL);
    }
}

void destroy(struct Lock* self){
   atomic_store(&self->tail, (_Atomic struct node*) NULL); 
}


