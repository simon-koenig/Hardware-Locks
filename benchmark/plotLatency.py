# Usual imports for plotting
# import numpy as np
# import matplotlib.pyplot as plt
# import sys
import os

# Read in Latency Text files from various Locks and store in array. V
directory = os.path.dirname(os.path.abspath(__file__))
text_files = [file_name for file_name in os.listdir(directory) if file_name.startswith("Latency")]
latency_array = []
lock_array = []

for file_name in text_files:
    file_path = os.path.join(directory, file_name)
    with open(file_path, 'r') as file:
        lines = file.readlines()
        if len(lines) >= 7:
            # Value is in nanoseconds
            value = lines[6].strip()
            latency_array.append( value )
            lock = file_name.split('Latency')[1].split('.txt')[0]
            lock_array.append(lock)

print(latency_array)
print(lock_array)

