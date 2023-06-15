# Usual imports for plotting
# import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys
import os

def plotSingleTP(data,color,label,marker):
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
    # print(threads)
    # print(means)
    # print(stdDevs)

    df = {"Threads": threads, "Means": means, "stdDevs": stdDevs}
    plt.plot(df["Threads"], df["Means"], label = label, marker = marker)# , color = color)
    plt.errorbar(df["Threads"], df["Means"], df["stdDevs"], linestyle='None', fmt='o', capsize=3)

def plotLockTPHigh():
    directory = os.path.dirname(os.path.abspath(__file__))
    data_files = [file_name for file_name in os.listdir(directory) if file_name.startswith("TPHighContention")]
    markers = ['+', '.', '*', 'o', 'v', 'x','^']

    for data_file, marker in zip(data_files ,markers):
        lock_name = data_file.split('TPHighContention')[1].split('.txt')[0]
        plotSingleTP(data=data_file, color="black",label=lock_name, marker=marker)
    #plt.title("Tp High Contention")
    plt.xlabel("Threads")
    plt.ylabel("Aggregate throughput rate: ops/sec")
    plt.yscale("log")
    plt.legend()
    plt.grid()
    plt.show()

def plotLockTPLow():
    directory = os.path.dirname(os.path.abspath(__file__))
    data_files = [file_name for file_name in os.listdir(directory) if file_name.startswith("TPLowContention")]
    markers = ['+', '.', '*', 'o', 'v', 'x','^']

    for data_file, marker in zip(data_files ,markers):
        lock_name = data_file.split('TPLowContention')[1].split('.txt')[0]
        plotSingleTP(data=data_file, color="black",label=lock_name, marker=marker)
    #plt.title("Tp High Contention")
    plt.xlabel("Threads")
    plt.ylabel("Aggregate throughput rate: ops/sec")
    plt.yscale("log")
    plt.legend()
    plt.grid()
    plt.show()
    


if sys.argv[1] == "high":
    plotLockTPHigh()

if sys.argv[1] == "low":
    plotLockTPLow()





