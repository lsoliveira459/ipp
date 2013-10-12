#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int width;
    int height;
    int* data;
} matrix;

int main(int argc, char** argv){
    int p;
    int my_rank;
    matrix a,b;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    init_matrix(my_rank,&a);

    printf("(r%dw%dh%d)\n",my_rank,a.width,a.height);

    print_matrix(&a);

    MPI_Finalize();
    return 0;
}

int init_matrix(int my_rank,matrix* m){
    if (my_rank == 0){
        printf("Enter the order of the matrix (h x w)\n");
        scanf("%d %d", &m->height, &m->width);
    }

    MPI_Bcast(&m->width, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&m->height, 1, MPI_INT, 0, MPI_COMM_WORLD);
   
    int i, s = m->width*m->height;

    if((m->data = (int*) malloc(sizeof(int)*s)) == NULL){
        printf("Erro de memoria");
        return 1;
    }
    for(i=0;i<s;i++)
        m->data[i] = i;

    return 0;
}

int print_matrix(matrix* m){
    const int   ii = m->height,
                jj = m->width;
    int i,j;
    for(i=0; i<ii; i++){
        for(j=0; j<jj; j++)
            printf("%4d",m->data[j+jj*i]);
        printf("\n");
    }
    return 0;
}
int global_sum(int p,float* op) {
    int p, r, i;
    float buf;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    
    if(r == 0){
        MPI_Status *status;
        
        for(i=1;i<p;i++) {
            MPI_Recv(&buf, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, status);
            *op += buf;
        }
    }
    else {
        MPI_Send(op, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
    }
}
int global_vector_sum(int p, float* vector,int vsize) {
    int p, r, i, j;
    float buf;
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    
    if(r == 0){
        MPI_Status *status;
        
        memset(vector,0,vsize)
        for(i=1;i<p;i++) {
            for(j=0; j<vsize;j++){
                MPI_Recv(&buf, 1, MPI_FLOAT, i, j, MPI_COMM_WORLD, status);
                vector[j] += buf;
            }
        }
    }
    else {
        for(j=0; j<vsize;j++){
            MPI_Send(&(vector[j]), 1, MPI_FLOAT, 0, j, MPI_COMM_WORLD);
        }
    }
}
