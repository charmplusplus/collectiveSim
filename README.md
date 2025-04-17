# Ping - Pong

A simple test to see when (under what scale of parameters) does zero copy become better than message passing. 

# Build and Run
```
make CHARMC=/path/to/charmc zerocopy
make CHARMC=/path/to/charmc message
```
This gives two executables `message_sim` and `zerocopy_sim`

The `run.sh` script will run all the ping pong program for all the data sizes. The runs will do two iterations of warmup and 3 actual iterations. On two seperate allocations run `./run.sh ./zerocopy_sim` and `./run.sh ./message_sim`


# Results


## Ping-Pong between 2 PEs on the same node
Allocate using 
```
salloc --partition=cpu-interactive --nodes=1 --ntasks-per-node=2 --cpus-per-task=1 --account=mzu-delta-cpu --time=00:30:00
```
and then run using the commands mentioned.


### MESSAGE PASSING

| Data    | Ping-Pongs | Trial Count | Average Time (s) |
|---------|------------|--------------|------------------|
| 1 KB    | 100000     | 10           | 2.530250         |
| 2 KB    | 100000     | 10           | 2.477156         |
| 4 KB    | 100000     | 10           | 3.124562         |
| 8 KB    | 100000     | 10           | 3.594205         |
| 16 KB   | 100000     | 10           | 4.195026         |
| 32 KB   | 100000     | 10           | 4.988685         |
| 64 KB   | 100000     | 10           | 7.269073         |
| 128  KB | 100000     | 10           | 7.690687         |
| 256  KB | 100000     | 10           | 11.966872        |
| 512  KB | 100000     | 10           | 13.548826        |
| 1024 KB | 100000     | 10           | 35.845900        |

### ZERO COPY

| Data    | Ping-Pongs | Trial Count  | Average Time (s) |
|---------|------------|--------------|------------------|
| 1 KB    | 100000     | 10           | 2.617608         |
| 2 KB    | 100000     | 10           | 2.706514         |
| 4 KB    | 100000     | 10           | 2.692925         |
| 8 KB    | 100000     | 10           | 2.717558         |
| 16 KB   | 100000     | 10           | 2.937580         |
| 32 KB   | 100000     | 10           | 3.085743         |
| 64 KB   | 100000     | 10           | 3.579777         |
| 128 KB  | 100000     | 10           | 4.379186         |
| 256 KB  | 100000     | 10           | 5.951493         |
| 512 KB  | 100000     | 10           | 9.808604         |
| 1024 KB | 100000     | 10           | 16.331971        |


<image src="plots/message_vs_zero_one.png" alt="Message Vs ZeroCopy on single Node">


## Ping-Pong between 2 PEs on different nodes
Allocate using 
```
salloc --partition=cpu-interactive --nodes=2 --ntasks-per-node=1 --cpus-per-task=1 --account=mzu-delta-cpu --time=00:30:00
```
and then run using the commands mentioned.


### MESSAGE PASSING

| Data    | Ping-Pongs | Trial Count  | Average Time (s) |
|---------|------------|--------------|------------------|
| 1 KB    | 100000     | 10           | 2.477801         |
| 2 KB    | 100000     | 10           | 2.487835         |
| 4 KB    | 100000     | 10           | 2.974495         |
| 8 KB    | 100000     | 10           | 3.137084         |
| 16 KB   | 100000     | 10           | 3.389508         |
| 32 KB   | 100000     | 10           | 4.120174         |
| 64 KB   | 100000     | 10           | 5.389932         |
| 128 KB  | 100000     | 10           | 7.392645         |
| 256 KB  | 100000     | 10           | 12.175158        |
| 512 KB  | 100000     | <BLOCKS>     | <BLOCKS>         |
| 1024 KB | 100000     | <BLOCKS>     | <BLOCKS>         |

### ZERO COPY

| Data    | Ping-Pongs | Trial Count  | Average Time (s) |
|---------|------------|--------------|------------------|
| 1 KB    | 100000     | 10           | 2.997686         |
| 2 KB    | 100000     | 10           | 2.986386         |
| 4 KB    | 100000     | 10           | 3.063087         |
| 8 KB    | 100000     | 10           | 3.199523         |
| 16 KB   | 100000     | 10           | 3.002056         |
| 32 KB   | 100000     | 10           | 3.047844         |
| 64 KB   | 100000     | 10           | 5.263045         |
| 128 KB  | 100000     | 10           | 8.488367         |
| 256 KB  | 100000     | 10           | 5.404486         |
| 512 KB  | 100000     | 10           | 8.167729         |
| 1024 KB | 100000     | 10           | 11.001834        |


<image src="plots/message_vs_zero_two.png" alt="Message Vs ZeroCopy on two Nodes">
