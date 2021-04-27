#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <string.h>
#include "mpi.h"

#define MAX_LINES 1000000
#define MAX_LINE_SIZE 2001
int NUM_THREADS;

char char_array[MAX_LINES][MAX_LINE_SIZE];
float out_put_array[MAX_LINES];
float local_char_array[MAX_LINES];

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
        strncpy(char_array[i] ,line, MAX_LINE_SIZE);
    }
    printf("file done\n");
    fclose( fd );

}

void *chunk_array(void *ID)
{
    int startPos = *((int *)(ID)) * (MAX_LINES / NUM_THREADS);
    int endPos = startPos + (MAX_LINES / NUM_THREADS);
    int i, j;
    for ( i = 0 ; i < endPos-startPos; i++ )
    {
        local_char_array[i] = find_avg(char_array[i+startPos],  strlen(char_array[i+startPos]));
    }

    //for ( i = 0 ; i < endPos-startPos; i++ )
    //{
     //   out_put_array[i+startPos] = find_avg(local_char_array[i], strlen(local_char_array[i]));
    //}



}

void print_results()
{
    int i;
    for ( i = 0; i < MAX_LINES; i++ )
    {
        printf("%d: %.1f\n", i, out_put_array[i] );
    }
}

int main(int argc, char* argv[])
{
    int i, numTask, rank, arc;
    MPI_Status Status;

    arc = MPI_Init(&argc, &argv);
    if (arc != MPI_SUCCESS)
    {
        printf("Error");
        MPI_Abort(MPI_COMM_WORLD, arc);
    }

    MPI_Comm_size(MPI_COMM_WORLD,&numTask);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);


    NUM_THREADS = numTask;
    printf("size = %d rank = %d\n", numTask, rank);
    fflush(stdout);

    if (rank == 0)
    {
        read_file();
    }
    MPI_Bcast(char_array, MAX_LINES * MAX_LINE_SIZE, MPI_CHAR, 0, MPI_COMM_WORLD);
        printf("MPI_Bcast\n");
    chunk_array(&rank);
        printf("cunk_array\n");
    MPI_Reduce(local_char_array, out_put_array, MAX_LINES, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);
        printf("MPI_Reduce\n");

    if (rank == 0)
    {   printf("befor print\n");
        print_results();
        printf("affter print\n");
    }

    MPI_Finalize();
}
