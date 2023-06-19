# Usual imports for plotting
# import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import sys
import os

def plotSingleTP(data,label,marker,color):
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


    df = {"Threads": threads, "Means": means, "stdDevs": stdDevs}
    plt.plot(df["Threads"], df["Means"], label = label, marker = marker, color = color)
    plt.errorbar(df["Threads"], df["Means"], df["stdDevs"], linestyle='None', marker=marker, capsize=2, color = color)

def plotLockTPHigh():
    # Get the current directory
    current_directory = os.path.dirname(os.path.abspath(__file__))
    # Get the parent directory
    parent_directory = os.path.dirname(current_directory)
    data_directory = os.path.join(parent_directory,"data","throughput")
    data_files = [file_name for file_name in os.listdir(data_directory) if file_name.startswith("TPHighContention")]
    markers = ['+', '.', '*', 'o', 'v', 'x','^','+']
    colors = ['red', "pink",'green', 'blue', 'yellow', "black", "purple",'grey']

    for data_file, marker, color in zip(data_files ,markers,colors):
        file_path = os.path.join(data_directory, data_file)
        lock_name = data_file.split('TPHighContention')[1].split('.txt')[0]
        plotSingleTP(data=file_path,label=lock_name, marker=marker, color=color)
    
    plt.xlabel("Threads")
    plt.ylabel("Aggregate throughput rate: ops/sec")
    plt.xscale("log")
    plt.yscale("log")
    plt.xticks([1,2,5,10,20,50,64],[1,2,5,10,20,50,64])
    plt.legend()
    plt.grid()
    plt.savefig("plots/TPHighContention.svg")
    plt.show()

def plotLockTPLow():
     # Get the current directory
    current_directory = os.path.dirname(os.path.abspath(__file__))
    # Get the parent directory
    parent_directory = os.path.dirname(current_directory)
    data_directory = os.path.join(parent_directory,"data","throughput")
    data_files = [file_name for file_name in os.listdir(data_directory) if file_name.startswith("TPLowContention")]
    markers = ['+', '.', '*', 'o', 'v', 'x','^','+']
    colors = ['red', "pink",'green', 'blue', 'yellow', "black", "purple","grey"]


    for data_file, marker, color  in zip(data_files ,markers ,colors):
        file_path = os.path.join(data_directory, data_file)
        lock_name = data_file.split('TPLowContention')[1].split('.txt')[0]
        plotSingleTP(data=file_path,label=lock_name, marker=marker,color=color)
    plt.xlabel("Threads")
    plt.ylabel("Aggregate throughput rate: ops/sec")
    plt.xscale("log")
    #plt.yscale("log")
    plt.xticks([1,2,5,10,20,50,64],[1,2,5,10,20,50,64])
    plt.legend()
    plt.grid()
    plt.show()
    plt.savefig("plots/TPLowContention.svg")
    


if __name__ == "__main__": 
    if sys.argv[1] == "high":
        plt.figure(figsize=(10,6))
        plotLockTPHigh()

    if sys.argv[1] == "low":
        plt.figure(figsize=(10,6))
        plotLockTPLow()





