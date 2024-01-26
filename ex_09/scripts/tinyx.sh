#!/bin/bash -l
#SBATCH --job-name=CA_EX8_stream
#SBATCH --gres=gpu:rtx3080:1
#SBATCH --cpu-freq=2200000
#SBATCH -t 02:00:00
#SBATCH -o /home/hpc/rzku/hpcv651h/gitrepo/CA_computer_exercise/ex_09/ex09_stream.out
#SBATCH -e /home/hpc/rzku/hpcv651h/gitrepo/CA_computer_exercise/ex_09/ex09_stream.err



# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
module load intel
module load likwid/5.3.0
# module load cuda

#srun likwid-pin -c E:S0:10 ../bin/stream >stream_on_cpu.csv
srun ../bin/jacobi >stream_on_gpu.csv

touch ready