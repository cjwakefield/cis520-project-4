#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include <sys/time.h>

#define MAX_LINES 1000000
#define MAX_LINE_SIZE 2001
//#define NUM_THREADS 16

char char_array[MAX_LINES][MAX_LINE_SIZE];
float out_put_array[MAX_LINES];
char local_char_array[MAX_LINES][MAX_LINE_SIZE];
pthread_mutex_t mutexcalc;
int NUM_THREADS; 

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

void *chunk_array(void *ID)
{
        printf("thread %d started\n", ((int)ID));
        //printf("got in\n");
        int startPos = ((int)ID) * (MAX_LINES / NUM_THREADS);
        //printf("start %d \n", startPos);
        int endPos = startPos + (MAX_LINES / NUM_THREADS);
        //printf("end : %d \n", endPos);
        int array_size = endPos-startPos;
        //printf("array size : %d \n", array_size);
        //char local_char_array[array_size][MAX_LINE_SIZE];
        //printf("char\n");
        int i, j;

        //printf("start : %d\n", startPos);
        //printf("end : %d\n", endPos);
        //pthread_mutex_lock (&mutexcalc);
        for ( i = 0 ; i < endPos-startPos; i++ )
        {
                //printf("cpy array%d\n" , i);
                strncpy(local_char_array[i+startPos] ,char_array[i+startPos], MAX_LINE_SIZE);
        }
        pthread_mutex_lock (&mutexcalc);

        for ( i = 0 ; i < endPos-startPos; i++ )
        {
                //printf("cpy array2%d\n", i);
                out_put_array[i+startPos] = find_avg(local_char_array[i+startPos], strlen(local_char_array[i+startPos]));
        }
        pthread_mutex_unlock (&mutexcalc);
         printf("thread %d ended\n", ((int)ID));
        pthread_exit(NULL);

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
	NUM_THREADS = atoi(getenv("SLURM_NTASKS"));
	printf("NUM_THREAD : %d" , NUM_THREADS);
        pthread_t threads[NUM_THREADS];
        pthread_attr_t attr;
        void *status;
        int i, rc;
        //These variables need to be initialized.
	struct timeval t1, t2;
	double elapsedTime;
	int numSlots, myVersion = 1;
        gettimeofday(&t1, NULL);
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_mutex_init(&mutexcalc, NULL);

        read_file();
	//gettimeofday(&t1, NULL);
        for ( i = 0 ; i <  NUM_THREADS; i++ )
        {
                rc = pthread_create(&threads[i], &attr, chunk_array, (void *)i);
                if (rc) {printf("error on thread creation\n"); exit(-1);        }

        }

        pthread_attr_destroy(&attr);
        for(i=0; i<NUM_THREADS; i++) {
                rc = pthread_join(threads[i], &status);
                 if (rc) {printf("error on thread death\n"); exit(-1);        }
        }
	//gettimeofday(&t2, NULL);
        print_results();
	gettimeofday(&t2, NULL);

	//Lastly, this goes at the end of the code.
	elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0; //sec to ms
	elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0; // us to ms
	printf("DATA, %d, %s, %f\n", myVersion, getenv("SLURM_NTASKS"),  elapsedTime);

	pthread_mutex_destroy(&mutexcalc);
	pthread_exit(NULL);

}
