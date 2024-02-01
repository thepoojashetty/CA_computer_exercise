#!/bin/bash -l
#SBATCH --job-name=CA_EX10_stream
#SBATCH --gres=gpu:rtx3080:1
#SBATCH --cpu-freq=2200000
#SBATCH -t 02:00:00
#SBATCH -o /home/hpc/rzku/hpcv651h/gitrepo/CA_computer_exercise/ex_10/ex10_stream.out
#SBATCH -e /home/hpc/rzku/hpcv651h/gitrepo/CA_computer_exercise/ex_10/ex10_stream.err


# Enable debug and verbose mode
set -x
set -v

module load cuda

srun ../bin/matmul_gpu > matmul_gpu.csv

touch ready