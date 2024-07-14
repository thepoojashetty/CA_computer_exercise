#!/bin/bash -l
#SBATCH --job-name=CA_EX10_dgemm_cpu
#SBATCH -N 1
#SBATCH --ntasks-per-node=1
#SBATCH --cpu-freq=2200000
#SBATCH --exclusive
#SBATCH -t 02:00:00
#SBATCH -o /home/hpc/rzku/hpcv720h/ex_10/matmul_cpu/ex10.out
#SBATCH -e /home/hpc/rzku/hpcv720h/ex_10/matmul_cpu/ex10.err

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel
module load mkl

srun ../bin/dgemm_cpu > matmul_cpu.txt

touch ready

