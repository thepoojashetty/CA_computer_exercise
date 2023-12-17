#!/bin/bash

for i in {1..31} 
do
    #result=$(echo "e($i*l(2^17)/31)" | bc -l)
    result=$(printf "%.0f" "$(echo "e($i*l(2^17)/31)" | bc -l)")
    echo "i=$i: $result"
done
