#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct MCS_node{
   _Atomic bool locked;
   char padding[64];
   struct MCS_node* next;
};

struct MCS_Lock{
   struct MCS_node* mynode;
   _Atomic (struct MCS_node*) head;
};

void MCS_init(struct MCS_Lock* self){
   atomic_store(&self->head, (struct MCS_node*) NULL);   
}

void MCS_lock(struct MCS_Lock* self){
    struct MCS_node* n = (struct MCS_node*)malloc(sizeof(struct MCS_node));
    atomic_store(&n->next, (struct MCS_node*) NULL);
    struct MCS_node* pred = atomic_exchange(&self->head, n);

    if (pred != (struct MCS_node*) NULL) {
        atomic_store(&n->locked, 1);   
        pred->next = n;
        while (atomic_load(&n->locked));
    } else {
        self->mynode = n;
    }
}

void MCS_unlock(struct MCS_Lock* self)
{
    struct MCS_node* n = self->mynode;
    if (n->next == (struct MCS_node*) NULL){
        if (atomic_compare_exchange_strong(&self->head, &n, (struct MCS_node*) NULL)) {
            free(n);
            return;
        }
        n = self->mynode;
        while (n->next == (struct MCS_node*) NULL);
    }
    atomic_store(&n->next->locked, 0);
    self->mynode = n->next;
    n->next = (struct MCS_node*) NULL;
    free(n);
}

void MCS_destroy_lock(struct MCS_Lock* self){
    free(atomic_load(&self->head));
}
