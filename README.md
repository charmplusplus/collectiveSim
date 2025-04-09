# Ping - Pong

A simple test to see when (under what scale of parameters) does zero copy become better than message passing. 

# Results

| Data  | Ping-Pongs | Message     | Zerocopy    | PE  | Nodes |
|-------|------------|-------------|-------------|-----|-------|
| 1 MB  | 100,000    | 39.990689s  | 24.259897s  | 20  | 1     |
| 1 MB  | 100,000    | 42.704204s  | 20.390878s  | 2   | 1     |
| 1 MB  | 100,000    | 32.231863s  | 10.949946s  | 2   | 2     |
| 1 MB  | 100,000    | 43.944832s  | 20.854975s  | 20  | 2     |
