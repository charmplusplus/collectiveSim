#!/bin/bash
# usage: ./run.sh <executable>

for bytes in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576; do
    kilo_bytes=$(( bytes / 1024 ))
    echo "--------------------------------------------------"
    echo "Running benchmark for: $kilo_bytes KB"
    
    data_value=$(( bytes / 4 ))
    sum=0

    for run in {1..3}; do
        output=$(./charmrun +p2 ./$1 "$data_value")
        time=$(echo "$output" | grep "Time Taken:" | awk '{print $3}')
        echo "    Extracted Time: $time seconds"
        sum=$(echo "$sum + $time" | bc -l)
    done

    average=$(echo "$sum / 3" | bc -l)

    echo "    Average Time: $average seconds"
done
