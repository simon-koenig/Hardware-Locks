# Imports
import numpy as np
import matplotlib.pyplot as plt
# import sys
import os

# Read in Latency Text files from various Locks and store in array. V
directory = os.path.dirname(os.path.abspath(__file__))
text_files = [file_name for file_name in os.listdir(directory) if file_name.startswith("Latency")]
latency_array = []
lock_array = []
stdDev_array = []

for file_name in text_files:
    file_path = os.path.join(directory, file_name)
    with open(file_path, 'r') as file:
        lines = file.readlines()
        if len(lines) >= 7:
            # Value is in nanoseconds
            _ = lines[6].strip()
            values = _.split(',')
            latency = values[0]
            stdDev = values[1]
            latency_array.append( float(latency) )
            stdDev_array.append( float(stdDev) )
            lock = file_name.split('Latency')[1].split('.txt')[0]
            lock_array.append(lock)
#print(latency_array)
#print(stdDev_array)
#print(lock_array)

# Build the plot
fig, ax = plt.subplots()
positions = np.arange(len(lock_array))
ax.bar(positions, latency_array, yerr=stdDev_array, align='center', alpha=0.5, ecolor='black', capsize=10)
ax.set_ylabel("Latency: ns")
ax.set_xticks(positions)
ax.set_xticklabels(lock_array, rotation=45, ha="right")
# ax.set_title("Latency comparison")
ax.yaxis.grid(True)

# Save the figure and show
plt.tight_layout()
#plt.savefig('bar_plot_with_error_bars.png')
plt.show()

