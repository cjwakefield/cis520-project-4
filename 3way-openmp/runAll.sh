#!/bin/bash

for i in 1 2 4 8 16
do	
	for k in 1 2 3 4 5 6 7 8 9 10
	do 
		sbatch --constraint=dwarves --ntasks-per-node=$i --nodes=1 --mem=16G --output "outputdata/core${i}openmp${k}.out" run.sh
	done
done
