#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <stdbool.h>
#include <omp.h>
#include "array-lock.c"

/* These structs should to match the definition in benchmark.py
 */
struct counters {
    int failed_turns;
    int successful_lends;
};
struct bench_result {
    float time;
    struct counters reduced_counters;
};

/* This is our example shared structure, a library for threads. Books can be
 * lent and returned via the four respective functions.
 */
struct one_thread_library {
    int art_of_multiprocessor_programming;
    int the_c_programming_language;
} library;


bool lend_herlihyshavit_luchangco_spear(struct one_thread_library* l) {
    if (l->art_of_multiprocessor_programming > 0) {
        l->art_of_multiprocessor_programming--;
        return true;
    }
    return false;
}

void return_herlihyshavit_luchangco_spear(struct one_thread_library* l) {
    l->art_of_multiprocessor_programming++;
}


struct counters random_bench1(struct one_thread_library* l, int times) {
    int tid = omp_get_thread_num();
    printf("Thread %d started.\n", tid);
    // Barrier to force OMP to start all threads at the same time
    #pragma omp barrier

    struct counters data = {.failed_turns = 0,
                            .successful_lends = 0};

    int herlihy_shavit_luchangco_spear = 0;

    for (int i=0; i<times;) { // times here is the number of times every thread enters the critical section
        // What are we reading today?

        /*
        printf("Thread %d: has %d AMP books and %d C books.\n",\
               tid, herlihy_shavit_luchangco_spear, kernigham_ritchie);
        */
        // We can only have exclusive access to the library
        // Here omp critical locks a built in lock. We have to replace this by our own implemented locks. 
        lock(); // Replace omp critical with custom lock 
        {
                if (lend_herlihyshavit_luchangco_spear(l)) {
                    herlihy_shavit_luchangco_spear++;
                    data.successful_lends++;
                } else if (herlihy_shavit_luchangco_spear > 0) {
                    return_herlihyshavit_luchangco_spear(l);
                    herlihy_shavit_luchangco_spear--;
                    i++;
                } else {
                    data.failed_turns++;
                }
            }
        unlock() ;
        }
    return data;
}

struct bench_result small_bench(int t, int len) {
    struct bench_result result;
    struct counters thread_data[t];
    double tic, toc;

    library.art_of_multiprocessor_programming = 20;
    library.the_c_programming_language = 20;

    omp_set_num_threads(t);
    tic = omp_get_wtime();
    {
        #pragma omp parallel for
        for (int i=0; i<t; i++) {
            thread_data[i] = random_bench1(&library, len);
        }
    }
    toc = omp_get_wtime();

    for (int i=0; i<t; i++) {
        result.reduced_counters.successful_lends += thread_data[i].successful_lends;
        result.reduced_counters.failed_turns     += thread_data[i].failed_turns;
    }

    result.time = (toc - tic);
    printf("LIBRARY: lending %d books on %d threads took: %fs\n",\
           len, t, toc - tic);
    printf("  with %d failed turns and %d successful lends: %f lends/s throughput\n",\
        result.reduced_counters.failed_turns,
        result.reduced_counters.successful_lends,
        result.reduced_counters.successful_lends/result.time);

    return result;
}

/* main is not relevant for benchmark.py but necessary when run alone for
 * testing.
 */
int main(int argc, char * argv[]) {
    (void) argc;
    (void) argv;
    small_bench(1, 100);
    small_bench(2, 100);
    small_bench(4, 100);
    small_bench(8, 100);
    small_bench(16, 100);
}
