#!/bin/bash -l
#SBATCH --job-name=CA_EX8_stream
#SBATCH -N 1
#SBATCH --ntasks-per-node=1
#SBATCH –gres=gpu:rtx3080:1
#SBATCH --exclusive
#SBATCH -t 02:00:00
#SBATCH -o /home/hpc/rzku/hpcv651h/ex_08/ex08_stream.out
#SBATCH -e /home/hpc/rzku/hpcv651h/ex_08/ex08_stream.err

# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel
module load cuda

# TODO allocate a compute node

srun likwid-pin -c N:0-7

touch ready

