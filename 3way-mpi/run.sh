#!/bin/bash -l 
##$ -l h_rt=0:01:00

module load OpenMPI

mpirun mpi.out

