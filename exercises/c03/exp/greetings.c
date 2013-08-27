/* greetings.c -- greetings program
 *
 * Send a message from all processes with rank != 0 to process 0.
 *    Process 0 prints the messages received.
 *
 * Input: none.
 * Output: contents of messages received by process 0.
 *
 * See Chapter 3, pp. 41 & ff in PPMPI.
 */
#include <stdio.h>
#include <string.h>
#include "mpi.h"

main(int argc, char* argv[]) {
    int         my_rank;       /* rank of process      */
    int         p;             /* number of processes  */
    int         source;        /* rank of sender       */
    int         dest;          /* rank of receiver     */
    int         tag = 0;       /* tag for messages     */
    char        message[100];  /* storage for message  */
    MPI_Status  status;        /* return status for    */
                               /* receive              */

    /* Start up MPI */
    MPI_Init(&argc, &argv);

    /* Find out process rank  */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out number of processes */
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    dest = (my_rank+1)%p;

	/* Create message */
    sprintf(message, "Greetings from %d to %d!",
	    my_rank, dest);

	/* Use strlen+1 so that '\0' gets transmitted */
	MPI_Send(message, strlen(message)+1, MPI_CHAR, 
	    dest, tag, MPI_COMM_WORLD);
	MPI_Recv(message, 100, MPI_CHAR, MPI_ANY_SOURCE, tag, 
	    MPI_COMM_WORLD, &status);
	printf("%s\n",message);

    /* Shut down MPI */
    MPI_Finalize();
} /* main */
