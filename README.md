# Ping - Pong

A simple test to see when (under what scale of parameters) does zero copy become better than message passing. 

# Results

| Data  | Ping-Pongs | Message     | Zerocopy    |
|-------|------------|-------------|-------------|
| 1 MB  | 100,000    | 39.990689s  | 24.259897s  |
| 1 MB  | 100,000    | 42.704204s  | 20.390878s  |
| 1 MB  | 100,000    | 32.231863s  | 10.949946s  |
| 1 MB  | 100,000    | 43.944832s  | 20.854975s  |
