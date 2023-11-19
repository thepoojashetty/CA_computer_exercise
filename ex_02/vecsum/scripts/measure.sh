#!/bin/bash -l
module load intel

sbatch ./meggie.sh

while [ ! -f ready ]; do
	sleep 60
done
rm ready

