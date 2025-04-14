# Ping - Pong

A simple test to see when (under what scale of parameters) does zero copy become better than message passing. 

# Results

## Ping-Pong between 2 PEs on the same node

| Data    | Ping-Pongs |             |   Message   |             |             |  Zerocopy   |             |
|---------|------------|-------------|-------------|-------------|-------------|-------------|-------------|
|         |            | Trial 1     | Trial 2     | Trial 3     | Trial 1     | Trial 2     | Trial 3     |
| 1 KB    | 100,000    |             |             |             |             |             |             |
| 100 KB  | 100,000    |             |             |             |             |             |             |
| 200 KB  | 100,000    |             |             |             |             |             |             |
| 400 KB  | 100,000    |             |             |             |             |             |             |
| 600 KB  | 100,000    |             |             |             |             |             |             |
| 800 KB  | 100,000    |             |             |             |             |             |             |
| 1 MB    | 100,000    |             |             |             |             |             |             |



## Ping-Pong between 2 PEs on different nodes

| Data    | Ping-Pongs |             |   Message   |             |             |  Zerocopy   |             |
|---------|------------|-------------|-------------|-------------|-------------|-------------|-------------|
|         |            | Trial 1     | Trial 2     | Trial 3     | Trial 1     | Trial 2     | Trial 3     |
| 1 KB    | 100,000    |             |             |             |             |             |             |
| 100 KB  | 100,000    |             |             |             |             |             |             |
| 200 KB  | 100,000    |             |             |             |             |             |             |
| 400 KB  | 100,000    |             |             |             |             |             |             |
| 600 KB  | 100,000    |             |             |             |             |             |             |
| 800 KB  | 100,000    |             |             |             |             |             |             |
| 1 MB    | 100,000    |             |             |             |             |             |             |
