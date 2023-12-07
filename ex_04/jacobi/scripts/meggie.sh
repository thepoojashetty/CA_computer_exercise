#!/bin/bash -l
#SBATCH --job-name=CA_EX4_jacobi
#SBATCH -N 1
#SBATCH --ntasks-per-node=1
#SBATCH --cpu-freq=2200000
#SBATCH --exclusive
#SBATCH -t 02:00:00
#SBATCH --constraint=hwperf
#SBATCH -o /home/hpc/rzku/hpcv720h/ex_04/jacobi/ex04_jacobi.out
#SBATCH -e /home/hpc/rzku/hpcv720h/ex_04/jacobi/ex04_jacobi.err

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel

#Load module for likwid
module load likwid/5.3.0

# TODO allocate a compute node

# This line creates / overrides a result csv file
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize" > result_row.csv

# TODO run benchmark 1
# execute measurement with for loop
# 32 measurement points, exponentially distributed: 1 KiB - 128 MiB, each with 1000 ms as minimal runtime
# results should be appended to the result.csv (see >> operator)
# root31(2^17) ^ i = e(i*ln(2^17)/31)

#ARRAY_SIZE=()
for i in {0..25}
do
    array_size=$((2**i))
    #ARRAY_SIZE+=($array_size)
    srun likwid-perfctr -O --stats \
                -C 0 \
                -c 0 \
                -f \
                -m \
                -g MEM_UOPS_RETIRED_LOADS_ALL:PMC1,MEM_LOAD_UOPS_RETIRED_L1_HIT:PMC2 \
                ../bin/jacobi $array_size 1000 >> result_row.csv
     #srun ../bin/jacobi $array_size 1000 >> result_col.csv
done
# Note: copy the result.csv to a local machine!

touch ready

