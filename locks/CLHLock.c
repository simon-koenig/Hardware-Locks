#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

// Source: Ideas from http://concurrencyfreaks.blogspot.com/2014/05/exchg-mutex-alternative-to-mcs-lock.html

typedef struct Node
{
    _Atomic char succ_must_wait;

} Node;

static __thread struct Node MyNode = {1};

typedef struct
{
    char padding[64];  // To avoid false sharing with the tail
    _Atomic (Node *) tail;
} Lock;


/*
 * Initializes the mutex, creating a sentinel node.
 *
 * Progress Condition: Wait-Free Population Oblivious
 */
void init(Lock * self)
{
    // We create the first sentinel node unlocked, with islocked=0
    Node node_ = {0};
    Node* node = &node_;
    atomic_store(&self->tail, node);
}


/*
 * Destroy the mutex, clearing all memory.
 * You must be sure that there are no other threads currently holding
 * the lock or attempting to.
 *
 * Progress Condition: Wait-Free Population Oblivious
 */
void destroy(Lock * self)
{
    atomic_store(&self->tail, (Node*) NULL);
}


/*
 * Locks the mutex for the current thread. Will wait for other threads
 * that did the atomic_exchange() before this one.
 *
 * Progress Condition: Blocking
 */
void lock(Lock * self)
{
    // Create the new node locked by default, setting islocked=1
    Node *mynode = &MyNode; // My own node 
    Node *prev = atomic_exchange(&self->tail, mynode); // Previous node 

    // This thread's node is now in the queue, so wait until it is its turn
    char prev_islocked = atomic_load_explicit(&prev->succ_must_wait, memory_order_relaxed); // The previous node has either 0 (no wait) or 1 (must wait) in succ_must_wait
    if (prev_islocked) { // If prev_islocke == 1, then wait 
        while (prev_islocked) {
            prev_islocked = atomic_load(&prev->succ_must_wait); // Check whether I have progressed to front of queue 
        }
    }
}


/*
 * Unlocks the mutex. Assumes that the current thread holds the lock on the
 * mutex.
 * 
 */
void unlock(Lock * self)
{
    Node *mynode = &MyNode; // My own node 
    atomic_store(&mynode->succ_must_wait, 0);
}