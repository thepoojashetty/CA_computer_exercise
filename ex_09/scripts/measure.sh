#!/bin/bash -l
# module load cuda

sbatch ./tinyx.sh

while [ ! -f ready ]; do
	sleep 60
done
rm ready