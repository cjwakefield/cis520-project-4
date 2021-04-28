#!/bin/bash

for i in 1 2 4 8 16
do
	 #sbatch --constraint=dwarves --ntasks-per-node=$i --nodes=1 openmp_sbatch.sh
	 sbatch --constraint=dwarves --ntasks-per-node=$i --nodes=1 3way-base/run.sh
	 sbatch --constraint=dwarves --ntasks-per-node=$i --nodes=1 3way-mpi/run.sh
	 sbatch --constraint=dwarves --ntasks-per-node=$i --nodes=1 3way-openmp/run.sh
	 sbatch --constraint=dwarves --ntasks-per-node=$i --nodes=1 3way-pthreads/run.sh
done
