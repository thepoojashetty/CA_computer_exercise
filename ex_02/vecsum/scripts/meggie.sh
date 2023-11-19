#!/bin/bash -l
#SBATCH --job-name=CA_EX1
#SBATCH -N 1
#SBATCH --ntasks-per-node=1
#SBATCH --cpu-freq=2200000
#SBATCH --exclusive
#SBATCH -t 02:00:00
#SBATCH -o /home/hpc/rzku/hpcv651h/ex_01/ex01.out
#SBATCH -e /home/hpc/rzku/hpcv651h/ex_01/ex01.err

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel

# TODO allocate a compute node

# This line creates / overrides a result csv file
echo "ArraySize,AdditionsPerSecond,ActualRuntime,MinimalRuntime" > result_1fold.csv
echo "ArraySize,AdditionsPerSecond,ActualRuntime,MinimalRuntime" > result_2fold.csv
echo "ArraySize,AdditionsPerSecond,ActualRuntime,MinimalRuntime" > result_3fold.csv
echo "ArraySize,AdditionsPerSecond,ActualRuntime,MinimalRuntime" > result_4fold.csv
echo "ArraySize,AdditionsPerSecond,ActualRuntime,MinimalRuntime" > result_8fold.csv

# TODO run benchmark 1
# execute measurement with for loop
# 32 measurement points, exponentially distributed: 1 KiB - 128 MiB, each with 1000 ms as minimal runtime
# results should be appended to the result.csv (see >> operator)
# input parameter:
# to run an executable:
# 	srun ../bin/vecSum [size of the vector in KiB]
FOLD_TYPE=2
for i in {0..2}
do
    array_size=$((2**i))
    echo $array_size
    if [ "$FOLD_TYPE" -eq 1 ]; then
        srun ../bin/vecSum "$array_size" 1000 >> result_1fold.csv
    elif [ "$FOLD_TYPE" -eq 2 ]; then
        srun ../bin/vecSum "$array_size" 1000 >> result_2fold.csv
    elif [ "$FOLD_TYPE" -eq 3 ]; then
        srun ../bin/vecSum "$array_size" 1000 >> result_3fold.csv
    elif [ "$FOLD_TYPE" -eq 4 ]; then
        srun ../bin/vecSum "$array_size" 1000 >> result_4fold.csv
    else
        srun ../bin/vecSum "$array_size" 1000 >> result_8fold.csv     
    fi
    
done
# Note: copy the result.csv to a local machine!

touch ready

