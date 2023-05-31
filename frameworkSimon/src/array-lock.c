#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>

// Set Flags, first one to true else false 
int n = 8; //
bool flags[16] = {true}; 

// Making an atomic counter
_Atomic int tail = 0;

// Thread local variable 
__thread int mySlot ;

void lock() {
    mySlot = __sync_fetch_and_add(&tail, 1) % 16;
    
    while (!flags[mySlot]) {
        // Wait until the flag at mySlot is set to true
    }
}
void unlock() {
    flags[mySlot] = false;
    flags[(mySlot + 1) % 16] = true;
}
