//Do not forget to add these include statements.
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int main() {
	//These variables need to be initialized.
	struct timeval t1, t2;
	double elapsedTime;
	int numSlots, myVersion = 1;

	//This is the timer starter, code goes after this.
	gettimeofday(&t1, NULL);

	//This is the timer ender, code goes before this.
	gettimeofday(&t2, NULL);

	//Lastly, this goes at the end of the code.
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, %d, %s, %f\n", myVersion, getenv("SLURM_NTASKS"),  elapsedTime);

}