# Usual imports for plotting
# import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
# import sys
import os

def plotLockTPHigh(data,color,label):
    # Read in Latency Text files from various Locks and store in array. V
    directory = os.path.dirname(os.path.abspath(__file__))
    #text_files = [file_name for file_name in os.listdir(directory) if file_name.startswith("TPHighContention")]
    data_file = data
    threads = []
    means = []
    stdDevs = []
    # for file_name in text_files:
    file_path = os.path.join(directory, data_file)
    with open(file_path, 'r') as file:
        lines = file.readlines()
        table_lines = lines[5:]  # Discard the first 5 lines
        for line in table_lines:
            values = line.strip().split(',')
            if len(values) == 3:
                threads.append(float(values[0]))
                means.append(float(values[1]))
                stdDevs.append(float(values[2]))
    print(threads)
    print(means)
    print(stdDevs)

    df_TPHigh = {"Threads": threads, "Means": means, "stdDevs": stdDevs}
    plt.plot(df_TPHigh["Threads"], df_TPHigh["Means"], label = label)# , color = color)
    plt.errorbar(df_TPHigh["Threads"], df_TPHigh["Means"], df_TPHigh["stdDevs"], linestyle='None', fmt='o', capsize=3)#, color = color)

def plotMultiLockTP():
    directory = os.path.dirname(os.path.abspath(__file__))
    data_files = [file_name for file_name in os.listdir(directory) if file_name.startswith("TPHighContention")]
    for data_file in data_files:
        lock_name = data_file.split('TPHighContention')[1].split('.txt')[0]
        plotSingleLockTP(data=data_file, color="black",label=lock_name)


plotLockTPHigh()
plt.title("Tp High Contention")
plt.xlabel("Threads-")
plt.ylabel("Aggregate throughput rate: ops/sec")
plt.yscale("log")
plt.legend()
plt.grid()
plt.show()




