import matplotlib.pyplot as plt

# Data sizes (KB)
sizes_kb = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]

# Message Passing Averages
avg_msg_pass = [
    2.530250, 2.477156, 3.124562, 3.594205,
    4.195026, 4.988685, 7.269073, 8.495627,
    40.196980, 63.703597, 95.832978
]

# Zero Copy Averages
avg_zero_copy = [
    2.617608, 2.706514, 2.692925, 2.717558,
    2.937580, 3.085743, 3.579777, 4.379186,
    5.951493, 9.808604, 16.331971
]

# Create the plot
fig, ax = plt.subplots(figsize=(9, 5))
ax.plot(sizes_kb, avg_msg_pass, marker='o', label='Message Passing')
ax.plot(sizes_kb, avg_zero_copy, marker='s', label='Zero Copy')

# Log scale for x-axis
ax.set_xscale('log', base=2)

# Labels and title
ax.set_xlabel('Data Size (KB)')
ax.set_ylabel('Average Time (seconds)')
ax.set_title('BLOCKS: Message Passing vs. Zero Copy (Log Scale)')

# Set custom x-ticks
ax.set_xticks(sizes_kb)
ax.set_xticklabels([f'{s} KB' for s in sizes_kb], rotation=45, ha='right')

# Show legend
ax.legend()

plt.tight_layout()
plt.show()
