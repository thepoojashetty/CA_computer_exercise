#!/bin/bash -l
#SBATCH --job-name=CA_EX6_jacobi
#SBATCH -N 1
#SBATCH --ntasks-per-node=1
#SBATCH --cpu-freq=2200000
#SBATCH --exclusive
#SBATCH -t 02:00:00
#SBATCH -o /home/hpc/rzku/hpcv720h/ex_06/jacobi/ex06_jacobi.out
#SBATCH -e /home/hpc/rzku/hpcv720h/ex_06/jacobi/ex06_jacobi.err

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel

# TODO allocate a compute node

# This line creates / overrides a result csv file
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize" > result_cb_xy.csv

# TODO run benchmark 1
# execute measurement with for loop
# 32 measurement points, exponentially distributed: 1 Mib - 16 GiB, each with 1000 ms as minimal runtime
# results should be appended to the result.csv (see >> operator)
#root31(2^14) ^ i = e(i*ln(2^14)/31)
# for i in {1..31}
# do
#     array_size=$(printf "%.0f" "$(echo "e($i*l(2^14)/31)" | bc -l)") #array size is now in Mib
#     srun ../bin/jacobi $array_size 1000 >> result_cb_xy.csv
# done
srun ../bin/jacobi 512 1000 >> result_cb_xy.csv

# Note: copy the result.csv to a local machine!

touch ready

