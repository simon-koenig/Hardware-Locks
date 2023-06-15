# Usual imports for plotting
import numpy as np
import matplotlib.pyplot as plt
import sys

lock = str(sys.argv[1])
with open('test.txt') as f:
    _ = f.readline()
    _, threads = f.readline().split(",")
    threads = int(threads)
    _, acqus = f.readline().split(",")
    acqus = int(acqus)
    _, reps = f.readline().split(",")
    reps = int(reps)
    _ = f.readline()
    _ = f.readline()
    data = []
    for i in range(0, threads):
        _, data_ = f.readline().split(",")
        data.append(float(data_))
    

_ = plt.bar(x=range(1, threads+1), height=data, bottom=0)
plt.xlabel("Thread ID")
plt.ylabel("Number of lock acquisations")

plt.savefig(lock + "_" + str(threads) + "_" + str(acqus) + "_" + str(reps) + ".svg")