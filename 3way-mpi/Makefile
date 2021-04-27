all: mpicc mpi

mpicc:
        bash -c "module load OpenMPI"

mpi: getAvg_MPI.c
        mpicc getAvg_MPI.c -o mpi.out