#!/bin/bash -l
# module load cuda

sbatch ./meggie.sh

while [ ! -f ready ]; do
	sleep 60
done
rm ready