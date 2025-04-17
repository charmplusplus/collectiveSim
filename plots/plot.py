import matplotlib.pyplot as plt

# Data sizes (KB)
sizes_kb = [1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024]

# Message Passing Averages (Same Node)
# avg_msg_pass = [
#     2.530250, 2.477156, 3.124562, 3.594205,
#     4.195026, 4.988685, 7.269073, 7.690687,
#     11.966872, 13.548826, 35.845900
# ]

# # Zero Copy Averages (Same Node)
# avg_zero_copy = [
#     2.617608, 2.706514, 2.692925, 2.717558,
#     2.937580, 3.085743, 3.579777, 4.379186,
#     5.951493, 9.808604, 16.331971
# ]

# # Message Passing Averages (Different Nodes)
avg_msg_pass = [
    2.477801, 2.487835, 2.974495, 3.137084,
    3.389508, 4.120174, 5.389932, 7.392645,
    12.175158, 15.548826, 37.845900
    # Missing data for 512 KB and 1024 KB
]

# Zero Copy Averages (Different Nodes)
avg_zero_copy = [
    2.997686, 2.986386, 3.063087, 3.199523,
    3.002056, 3.047844, 5.263045, 8.488367,
    5.404486, 8.167729, 11.001834
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
ax.set_title('Message Passing vs. Zero Copy (nodes = 2, PE/node = 1)')

# Set custom x-ticks
ax.set_xticks(sizes_kb)
ax.set_xticklabels([f'{s} KB' for s in sizes_kb], rotation=45, ha='right')

# Show legend
ax.legend()

plt.tight_layout()
plt.show()
