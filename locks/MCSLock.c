#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct node{
   _Atomic bool locked;
   char padding[64];
   struct node* next;
} node;

typedef struct Lock{
   struct node* mynode;
   _Atomic (struct node*) head;
} Lock;

void init(struct Lock* self){
   atomic_store(&self->head, (struct node*) NULL);   
}

void lock(struct Lock* self){
    struct node* n = (struct node*)malloc(sizeof(struct node));
    atomic_store(&n->next, (struct node*) NULL);
    struct node* pred = atomic_exchange(&self->head, n);

    if (pred != (struct node*) NULL) {
        atomic_store(&n->locked, 1);   
        pred->next = n;
        while (atomic_load(&n->locked));
    } else {
        self->mynode = n;
    }
}

void unlock(struct Lock* self)
{
    struct node* n = self->mynode;
    if (n->next == (struct node*) NULL){
        if (atomic_compare_exchange_strong(&self->head, &n, (struct node*) NULL)) {
            free(n);
            return;
        }
        n = self->mynode;
        while (n->next == (struct node*) NULL);
    }
    atomic_store(&n->next->locked, 0);
    self->mynode = n->next;
    n->next = (struct node*) NULL;
    free(n);
}

void destroy(struct Lock* self){
    free(atomic_load(&self->head));
}

