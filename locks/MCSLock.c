#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node{
   _Atomic bool locked;
   struct node* next;
   char padding[64];
} node;

typedef struct Lock{
   _Atomic (struct node*) head;
} Lock;

static __thread struct node mynode = {0, (struct node*) NULL, ""};

void init(struct Lock* self){
   atomic_store(&self->head, (struct node*) NULL);   
}

void lock(struct Lock* self){
    struct node* n = &mynode;
    atomic_store(&n->next, (struct node*) NULL);
    struct node* pred = atomic_exchange(&self->head, n);

    if (pred != (struct node*) NULL) {
        atomic_store(&n->locked, 1);   
        pred->next = n;
        while (atomic_load(&n->locked));
    }
}

void unlock(struct Lock* self)
{
    struct node* n = &mynode;
    if (n->next == (struct node*) NULL){
        if (atomic_compare_exchange_strong(&self->head, &n, (struct node*) NULL)) {
            return;
        }
        n = &mynode;
        while (n->next == (struct node*) NULL);
    }
    atomic_store(&n->next->locked, 0);
    n->next = (struct node*) NULL;
}

void destroy(struct Lock* self){
    free(atomic_load(&self->head));
}

