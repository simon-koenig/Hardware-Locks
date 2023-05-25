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


typedef struct
{
    Node * mynode;
    char padding[64];  // To avoid false sharing with the tail
    _Atomic (Node *) tail;
} CLHMutex;


static Node * clh_create_node(char islocked)
{
    Node * new_node = (Node *)malloc(sizeof(Node));
    atomic_store_explicit(&new_node->succ_must_wait, islocked, memory_order_relaxed); // Sets the value of succ_must_wait to either 0 (no wait) or 1 (must wait)
    return new_node;
}

/*
 * Initializes the mutex, creating a sentinel node.
 *
 * Progress Condition: Wait-Free Population Oblivious
 */
void clh_init(CLHMutex * self)
{
    // We create the first sentinel node unlocked, with islocked=0
    Node * node = clh_create_node(0);
    self->mynode = node;
    atomic_store(&self->tail, node);
}


/*
 * Destroy the mutex, clearing all memory.
 * You must be sure that there are no other threads currently holding
 * the lock or attempting to.
 *
 * Progress Condition: Wait-Free Population Oblivious
 */
void clh_destroy(CLHMutex * self)
{
    free(atomic_load(&self->tail));
}


/*
 * Locks the mutex for the current thread. Will wait for other threads
 * that did the atomic_exchange() before this one.
 *
 * Progress Condition: Blocking
 */
void clh_lock(CLHMutex * self)
{
    // Create the new node locked by default, setting islocked=1
    Node *mynode = clh_create_node(1); // My own node 
    Node *prev = atomic_exchange(&self->tail, mynode); // Previous node 

    // This thread's node is now in the queue, so wait until it is its turn
    char prev_islocked = atomic_load_explicit(&prev->succ_must_wait, memory_order_relaxed); // The previous node has either 0 (no wait) or 1 (must wait) in succ_must_wait
    if (prev_islocked) { // If prev_islocke == 1, then wait 
        while (prev_islocked) {
            prev_islocked = atomic_load(&prev->succ_must_wait); // Check whether I have progressed to front of queue 
        }
    }
    // This thread has acquired the lock on the mutex and it is now safe to
    // cleanup the memory of the previous node.
    free(prev);

    // Could maybe replace this with a thread local variable 
    self->mynode = mynode; // This is set for later unlock()
}


/*
 * Unlocks the mutex. Assumes that the current thread holds the lock on the
 * mutex.
 * 
 */
void clh_unlock(CLHMutex * self)
{
    // We assume that if this function was called, it is because this thread is
    // currently holding the lock, which means that self->mynode is pointing to
    // the current thread's mynode.
    if (self->mynode == NULL) {
        // ERROR: This will occur if unlock() is called without a lock()
        return;
    }
    atomic_store(&self->mynode->succ_must_wait, 0);
}