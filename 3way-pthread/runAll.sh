#!/bin/bash

for i in 1 2 4 8 16
do	
	for k in 1 2 3 4 5 6 7 8 9 10
	do
		for j in 1 
		do 
			sbatch --constraint=elves --ntasks-per-node=${i} --nodes=${j} --mem=48G --output "outputdata/core${i}pthread${k}node${j}.out" run.sh
		done
	done
done
