#!/bin/bash -l
#SBATCH --job-name=CA_EX8_stream
#SBATCH -N 1
#SBATCH --ntasks=1
#SBATCH --gres=gpu:rtx3080:1
#SBATCH --exclusive
#SBATCH --time 00:30:00
#SBATCH -o /home/hpc/rzku/hpcv651h/gitrepo/CA_computer_exercise/ex_08/ex08_stream.out
#SBATCH -e /home/hpc/rzku/hpcv651h/gitrepo/CA_computer_exercise/ex_08/ex08_stream.err


# Enable debug and verbose mode
set -x
set -v

# Load module with icc compiler
# module load intel
# module load cuda


#salloc.tinygpu --gres=gpu:rtx3080:1 --time=01:00:00
#likwid -pin -c E:S0:10:1:1 ../bin/strbenchmark
#srun ../bin/stream
srun ../bin/strtriad
touch ready

