Project about Hardware Locks for "Advanced Multiprocessor Programming", summer term 2023 at TU WIEN. 

Contributors:
* Simon König, 11702826
* Viktor Beck, 11713110
* Michael Ketter, 11701297

# Project descripton

One of the main challenges in multiprocessor programming is to make sure that shared datastructures are read from and written to in a sequential-like manner. A universal way how this can be achieved for arbitrary datastructures is to generate a lock which makes sure that only one thread can access at a time. Since reading and writing to a shared datastructure in a given algorithm is done quite frequently, it is important that the actual implementation of the used lock is as efficient and fast as possible. Therefore, implementations of such locks often utilize hardware supported operations to optimize the efficiency even further. 

In this project, several implementations of locks using hardware supported operations should be implemented correctly. Furthermore, the performance and fairness parameter of the locks with varying thread number should be compared.

# Dependencies

To run all the scripts one needs to install the following packages (also GCC and Python3):
* Numpy
* Matplotlib
* Pandas

Installation can be done with 
```sh
$ make install
```
# Usage

Compile and run a throughput benchmark for low contention for all the locks implemented (except incorrect lock):
```sh
$ make small-bench
```
To plot the data generated run the following:
```sh
$ make small-plot
```
Explanation: 
We choose throughput low contention as an example benchmark to show the importance behaviour of locks in multi-thread environments. 
In low contention cases we gain an advantage in using multiple threads over a single thread. 



One can also specify the input parameters: 'threads', 'repetitions', 'sampleSize' and 'server' (parameter 'server' is only used for local testing - just add 'server=' to make command and it will work locally). E.g.: 
```sh
$ make small-bench sampleSize=10000 threads=8
```

Correctness test and benchmarks can also be compiled and executed individually:


### **Correctness:**
Check locks for correctness:
```sh
$ make correctnessTest
```

### **Benchmarks:**
Benchmarking:
```sh
$ make fairness
```
```sh
$ make tp-high-contention
```
```sh
$ make tp-low-contention
```
```sh
$ make latency
```
### **All:**
For the correctness test and all the benchmarks run: 

```sh
$ make bench
```

### **Plotting:**
Plot all the data in data folder:
```sh
$ make plot
```

### **Cleaning:**


```sh
$ make clean
```
```sh
$ make clean-plots
```
```sh
$ make clean-data
```


# Folder structure
```sh
├── build
├── data
│   ├── correctness
│   ├── fairness
│   ├── latency
│   └── throughput
├── locks
│   ├── ArrayLock.c
│   ├── BaselineLock.c
│   ├── CLHLock.c
│   ├── HemLock.c
│   ├── IncorrectLock.c
│   ├── MCSLock.c
│   ├── TestAndSetLock.c
│   ├── TestAndTestAndSetLock.c
│   └── TicketLock.c
└── src
    ├── correctnessTest.c
    ├── fairness.c
    ├── latency.c
    ├── plotFairness.py
    ├── plotLatency.py
    ├── plotTP.py
    ├── tp-high-contention.c
    ├── tp-low-contention.c
    └── utils.c
├── AMP_Programming_Assignment.pdf
├── LICENSE
├── Makefile
├── Projects.pdf
├── README

```

