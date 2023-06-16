#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Source: Ideas from http://concurrencyfreaks.blogspot.com/2014/05/exchg-mutex-alternative-to-mcs-lock.html

typedef struct Node
{
    _Atomic bool succ_must_wait;
    struct Node* next;
    char padding[64];
} Node;

static __thread struct Node* MyNode;

typedef struct
{
    Node dummy;
    _Atomic (Node *) tail;
} Lock;


void init(Lock * self)
{
    // We create the first sentinel node unlocked, with islocked=0
    Node node = {0, (Node*) NULL, ""};
    self->dummy = node;
    atomic_store(&self->tail, &self->dummy);
}


void destroy(Lock * self)
{
    atomic_store(&self->tail, (Node*) NULL);
}

void lock(Lock * self)
{  
    MyNode = (Node*)malloc(sizeof(struct Node)); // My own node
    atomic_store(&MyNode->succ_must_wait, 1);    
    MyNode->next = (Node*) atomic_exchange_explicit(&self->tail, MyNode, memory_order_seq_cst);
    while (atomic_load(&MyNode->next->succ_must_wait)) {}
}


void unlock(Lock * self)
{
    atomic_store(&MyNode->succ_must_wait, 0);
}