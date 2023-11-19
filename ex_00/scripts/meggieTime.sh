#!/bin/bash -l
#SBATCH --job-name=CA_EX0
#SBATCH -N 1
#SBATCH --ntasks-per-node=1
#SBATCH --cpu-freq=2200000
#SBATCH --exclusive
#SBATCH -t 02:00:00

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel

# TODO allocate a compute node

# This line creates / overrides a result csv file
echo "ArraySize,AdditionsPerSecond,ActualRuntime,MinimalRuntime" > result.csv

# TODO run benchmark 1
# execute measurement with for loop
# 32 measurement points, exponentially distributed: 1 KiB - 128 MiB, each with 1000 ms as minimal runtime
# results should be appended to the result.csv (see >> operator)
# input parameter:
# to run an executable:
# 	srun ../bin/vecSum [size of the vector in KiB]
time=0
for i in {0..20}
do
    time=$((time+50))
    echo $array_size
    srun ../bin/vecSum 1 $time >> result.csv
done

NumEle=20
AdditionsPerSecondCol=$(awk -F',' -v col=2 'NR > 1 {print $col}' result.csv)
echo "$AdditionsPerSecondCol"
standardDeviation=$(
    echo "$AdditionsPerSecondCol" |
        awk '{sum+=$1; sumsq+=$1*$1}END{print sqrt(sumsq/NR - (sum/NR)**2)}'
)
echo $standardDeviation

# Note: copy the result.csv to a local machine!

touch ready

