#!/bin/bash -l
#SBATCH --job-name=CA_EX9_stream
#SBATCH --gres=gpu:rtx3080:1
#SBATCH --cpu-freq=2200000
#SBATCH -t 02:00:00
#SBATCH -o /home/hpc/rzku/hpcv720h/ex_09/ex09_stream.out
#SBATCH -e /home/hpc/rzku/hpcv720h/ex_09/ex09_stream.err


# Enable debug and verbose mode
set -x
set -v

module load cuda

srun ../bin/stream_gpu >jacobi_100ms.csv

touch ready