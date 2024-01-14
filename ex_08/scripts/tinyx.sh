#!/bin/bash -l
#SBATCH --job-name=CA_EX8_stream
#SBATCH -N 1
#SBATCH --ntasks=1
#SBATCH --gres=gpu:rtx3080:1
#SBATCH --exclusive
#SBATCH -t 02:00:00
#SBATCH -o /home/hpc/rzku/hpcv651h/ex_08/ex08_stream.out
#SBATCH -e /home/hpc/rzku/hpcv651h/ex_08/ex08_stream.err


# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
# module load intel
# module load cuda


#salloc.tinygpu --gres=gpu:rtx3080:1 --time=00:30:00
srun ../bin/stream

touch ready

