# Usual imports for plotting
import numpy as np
import matplotlib.pyplot as plt
import sys
import os


# Read in Latency Text files from various Locks and store in array. 
# Get the current directory
current_directory = os.path.dirname(os.path.abspath(__file__))
# Get the parent directory
parent_directory = os.path.dirname(current_directory)
data_directory = os.path.join(parent_directory,"data","fairness")
text_files = [file_name for file_name in os.listdir(data_directory) if file_name.startswith("Fairness")]


#lock = str(sys.argv[1])
for file_name in text_files:
    file_path = os.path.join(data_directory, file_name)
    with open(file_path, 'r') as f:
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
    lock = file_name.split('Fairness')[1].split('.txt')[0]

