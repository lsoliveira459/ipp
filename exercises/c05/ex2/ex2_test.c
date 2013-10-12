#include "mpi.h"
#include <stdio.h>

int main(int argc,char** argv){
    int my_rank;
    int p;
    MPI_Status status;

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&p);

    int x,y,z;

    printf("proc %d\n",my_rank);
    
    switch(my_rank) {
	case 0: x = 0; y = 1; z = 2;
	    MPI_Bcast(&x,1,MPI_INT,0,MPI_COMM_WORLD);
	    MPI_Send(&y,1,MPI_INT,2,43,MPI_COMM_WORLD);
	    MPI_Bcast(&z,1,MPI_INT,1,MPI_COMM_WORLD);
	    break;
	case 1: x = 3; y = 4; z = 5;
	    MPI_Bcast(&x,1,MPI_INT,0,MPI_COMM_WORLD);
	    MPI_Bcast(&y,1,MPI_INT,1,MPI_COMM_WORLD);
	    break;
	case 2: x = 6; y = 7; z = 8;
	    MPI_Bcast(&z,1,MPI_INT,0,MPI_COMM_WORLD);
	    MPI_Recv(&x,1,MPI_INT,0,43,MPI_COMM_WORLD,&status);
	    MPI_Bcast(&y,1,MPI_INT,1,MPI_COMM_WORLD);
	    break;
    }

    printf("%d %d %d %d\n",my_rank,x,y,z);

    return 0;
}
