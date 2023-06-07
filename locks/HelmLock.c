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
   _Atomic struct node* mynode;
   _Atomic struct node* tail;
} Lock;

void init(struct Lock* self){
   atomic_store(&self->tail, (_Atomic struct node*) NULL); 
   atomic_store(&self->mynode, (_Atomic struct node*) NULL);
}

void lock(struct Lock* self){
    struct node* n = (struct node*)malloc(sizeof(struct node));
    atomic_store(&n->Grant, (_Atomic struct Lock*) NULL); 
    struct node* pred = (struct node*) atomic_exchange(&self->tail, (_Atomic struct node*) n);

    if (pred != (struct node*) NULL) {
        while (atomic_load(&pred->Grant) != (_Atomic struct Lock*) self);
        atomic_store(&pred->Grant, (_Atomic struct Lock*) NULL);
    }
    atomic_store(&self->mynode, (_Atomic struct node*) n);
}

void unlock(struct Lock* self)
{
    struct node* n = (struct node*) atomic_load(&self->mynode);
    atomic_store(&n->Grant, (_Atomic struct Lock*) NULL); 
    struct node* pred = (struct node*) atomic_load(&self->tail);

    if (pred != n){
        atomic_store(&n->Grant, (_Atomic struct Lock*) self);
        while(atomic_load(&n->Grant) != (_Atomic struct Lock*) NULL);
    }
    free(n);
}

void destroy(struct Lock* self){
   atomic_store(&self->tail, (_Atomic struct node*) NULL); 
   atomic_store(&self->mynode, (_Atomic struct node*) NULL);
}


