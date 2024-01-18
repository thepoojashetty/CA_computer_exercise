#!/bin/bash -l
#SBATCH --job-name=CA_EX7_jacobi
#SBATCH -N 1
#SBATCH --ntasks-per-node=1
#SBATCH --cpu-freq=2200000
#SBATCH --exclusive
#SBATCH --cpus-per-task=20
#SBATCH -t 02:00:00
#SBATCH -o /home/hpc/rzku/hpcv720h/ex_07/jacobi/ex07_jacobi.out
#SBATCH -e /home/hpc/rzku/hpcv720h/ex_07/jacobi/ex07_jacobi.err

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel

module load likwid/5.3.0

# TODO allocate a compute node

# This line creates / overrides a result csv file
echo "ArraySize,MegaUpdatesPerSecond,ActualRuntime,MinimalRuntime,EdgeSize,NumberOfThreads" > result_thread_likwidpin.csv

# TODO run benchmark 1
# execute measurement with for loop
# 32 measurement points, exponentially distributed: 1 Mib - 16 GiB, each with 1000 ms as minimal runtime
# results should be appended to the result.csv (see >> operator)
#root31(2^14) ^ i = e(i*ln(2^14)/31)

#for 4GiB
for i in {1..20}; do
    if [ $i -le 10 ]; then
        likwid_option="E:S0:$i:1:1"
    else
        cores=$(($i-10))
        likwid_option="E:S0:10@S1:$cores"
    fi 
    likwid-pin -q -c $likwid_option ../bin/jacobi 4096 1000 $i >>result_thread_likwidpin.csv
# srun ../bin/jacobi 1 1000 3 >>result_thread_likwidpin.csv
done

# Note: copy the result.csv to a local machine!

touch ready

