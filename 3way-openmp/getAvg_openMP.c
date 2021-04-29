#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>
#include "omp.h"

#define MAX_LINES 1000000
#define MAX_LINE_SIZE 2001
#define NUM_THREADS 4

char char_array[MAX_LINES][MAX_LINE_SIZE];
float out_put_array[MAX_LINES];
char local_char_array[MAX_LINES][MAX_LINE_SIZE];

float find_avg(char* line, int nchars) {
    int i, j;
    float sum = 0;

    for ( i = 0; i < nchars; i++ ) {
        sum += ((int) line[i]);
    }

    if (nchars > 0)
        return sum / (float) nchars;
    else
        return 0.0;
}

void read_file()
{
    char *line = malloc(MAX_LINE_SIZE);
    FILE *fd;
    int i, err;


    fd = fopen( "/homes/dan/625/wiki_dump.txt", "r" );
    for ( i = 0; i < MAX_LINES; i++ )  {
        err = fscanf( fd, "%[^\n]\n", line);
        if( err == EOF ) break;
        //printf("working\n" );
        strncpy(char_array[i] ,line, MAX_LINE_SIZE);
        //printf("%d: %.1f\n", i, find_avg(char_array[i], strlen(char_array[i]) ));
    }
    printf("file done\n");
    fclose( fd );

}

void chunk_array(int ID)
{
    int startPos = ID * (MAX_LINES / NUM_THREADS);
    int endPos = startPos + (MAX_LINES / NUM_THREADS);
    int i;
    //char local_char_array[MAX_LINES/NUM_THREADS][MAX_LINE_SIZE];

    #pragma omp private(ID, startPos, endPos, i, local_char_array, out_put_array)
    {
	//#pragma omp critical
	//{
        for ( i = 0 ; i < endPos-startPos; i++ )
        {
            //printf("cpy array%d\n" , i);
            strncpy(local_char_array[i+startPos] ,char_array[i+startPos], MAX_LINE_SIZE);
        }
	#pragma omp critical
	{
            for ( i = 0 ; i < endPos-startPos; i++ )
            {
                //printf("cpy array2%d\n", i);
                out_put_array[i+startPos] = find_avg(local_char_array[i+startPos], strlen(local_char_array[i+startPos]));
            }
        }
    }
}
void print_results()
{
    int i;
    for ( i = 0; i < MAX_LINES; i++ )
    {
        printf("%d: %.1f\n", i, out_put_array[i] );
    }
}

int main()
{
    omp_set_num_threads(NUM_THREADS);
    //These variables need to be initialized.
	struct timeval t1, t2;
	double elapsedTime;
	int numSlots, myVersion = 1;
    int i ;
    //This is the timer starter, code goes after this.
    gettimeofday(&t1, NULL);
    read_file();
    //gettimeofday(&t1, NULL);
    #pragma omp parallel
    {
            chunk_array(omp_get_thread_num());
    }
    //gettimeofday(&t2, NULL);
    print_results();
    gettimeofday(&t2, NULL);
	//Lastly, this goes at the end of the code.
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to mmeofday(&t2, NULL);s
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, %d, %s, %f\n", myVersion, getenv("SLURM_NTASKS"),  elapsedTime);
}
