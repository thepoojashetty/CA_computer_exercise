#!/bin/bash -l
#SBATCH --job-name=CA_EX1
#SBATCH -N 1
#SBATCH --ntasks-per-node=1
#SBATCH --cpu-freq=2200000
#SBATCH --exclusive
#SBATCH -t 02:00:00
#SBATCH -o /home/hpc/rzku/hpcv720h/ex_01/ex01.out
#SBATCH -e /home/hpc/rzku/hpcv720h/ex_01/ex01.err

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel

# TODO allocate a compute node

# This line creates / overrides a result csv file
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize" > result.csv

# TODO run benchmark 1
# execute measurement with for loop
# 32 measurement points, exponentially distributed: 1 KiB - 128 MiB, each with 1000 ms as minimal runtime
# results should be appended to the result.csv (see >> operator)

#for i in {0..32}
#do
#    array_size=$((2**i))
#    echo $array_size
#    srun ../bin/jacobi $array_size 1000 >> result.csv
srun ../bin/jacobi 1 1000 >> result.csv
#done
# Note: copy the result.csv to a local machine!

touch ready

