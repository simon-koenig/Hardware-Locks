#include <stdatomic.h>
#include <unistd.h>
#include <omp.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Lock {} Lock;

void init(Lock* self) {}

void lock(Lock* self) {}

void unlock(Lock* self) {}

void destroy(Lock* self) {}
