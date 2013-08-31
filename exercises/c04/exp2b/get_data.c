#include <stdio.h>
#include "mpi.h"

main(int argc, char** argv) {
    int         my_rank;   /* My process rank           */
    int         p;         /* The number of processes   */
    float       a;         /* Left endpoint             */
    float       b;         /* Right endpoint            */
    int         n;         /* Number of trapezoids      */
    int         f;         /* Function identifier       */
    float       h;         /* Trapezoid base length     */
    float       local_a;   /* Left endpoint my process  */
    float       local_b;   /* Right endpoint my process */
    int         local_n;   /* Number of trapezoids for  */
                           /* my calculation            */
    float     (*local_f)(float); /* Pointer to function */
    float       integral;  /* Integral over my interval */
    float       total;     /* Total integral            */
    int         source;    /* Process sending integral  */
    int         dest = 0;  /* All messages go to 0      */
    int         tag = 0;
    MPI_Status  status;

    void Get_data(float* a_ptr, float* b_ptr, 
         int* n_ptr, int* f_ptr, int my_rank, int p);
    float Trap(float local_a, float local_b, int local_n, float (*f)(float),
              float h);    /* Calculate local integral  */

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    Get_data(&a, &b, &n, &f, my_rank, p);

    float k(float);
    float l(float);
    float m(float);

    switch(f){
        case 0: local_f = k;break;
        case 1: local_f = l;break;
        case 2:
       default: local_f = m;break;
    }

    h = (b-a)/n;    /* h is the same for all processes */
    local_n = n/p;  /* So is the number of trapezoids */

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n*h;
    integral = Trap(local_a, local_b, local_n,local_f, h);

    if (my_rank == 0) {
        total = integral;
        for (source = 1; source < p; source++) {
            MPI_Recv(&integral, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
            total = total + integral;
        }
    } else {   
        MPI_Send(&integral, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
    }

    if (my_rank == 0) {
        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %f\n", a, b, total); 
    }

    MPI_Finalize();
}

void Get_data(
         float*  a_ptr    /* out */, 
         float*  b_ptr    /* out */, 
         int*    n_ptr    /* out */,
         int*    f_ptr    /* out */,
         int     my_rank  /* in  */, 
         int     p        /* in  */) {

    int source = 0;    /* All local variables used by */
    int dest;          /* MPI_Send and MPI_Recv       */
    int tag;
    MPI_Status status;

    if (my_rank == 0){
        printf("Enter a, b, n, and f\n");
        scanf("%f %f %d %d", a_ptr, b_ptr, n_ptr, f_ptr);
        for (dest = 1; dest < p; dest++){
            tag = 0;
            MPI_Send(a_ptr, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
            tag = 1;
            MPI_Send(b_ptr, 1, MPI_FLOAT, dest, tag, MPI_COMM_WORLD);
            tag = 2;
            MPI_Send(n_ptr, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
            tag = 3;
            MPI_Send(f_ptr, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
        }
    } else {
        tag = 0;
        MPI_Recv(a_ptr, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
        tag = 1;
        MPI_Recv(b_ptr, 1, MPI_FLOAT, source, tag, MPI_COMM_WORLD, &status);
        tag = 2;
        MPI_Recv(n_ptr, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        tag = 3;
        MPI_Recv(f_ptr, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
    }
} /* Get_data */


/********************************************************************/
float Trap(
          float  local_a   /* in */, 
          float  local_b   /* in */, 
          int    local_n   /* in */,
          float  (*local_f)(float)/* in */,
          float  h         /* in */
        ) { 

    float integral;   /* Store result in integral  */ 
    float x; 
    int i;

    integral = ((*local_f)(local_a) + (*local_f)(local_b)); 
    x = local_a; 
    for (i = 1; i <= local_n-1; i++) { 
        x = x + h; 
        integral = integral + (i%2?4:2)*(*local_f)(x); 
    } 
    integral = integral*h/3.0; 
    return integral;
} /*  Trap  */


/********************************************************************/
float k(float x) { 
    float return_val; 
    /* Calculate f(x). */
    /* Store calculation in return_val. */ 
    return_val = x*x;
    return return_val; 
} /* k */
float l(float x) { 
    float return_val; 
    /* Calculate f(x). */
    /* Store calculation in return_val. */ 
    return_val = x*x+1;
    return return_val; 
} /* l */
float m(float x) { 
    float return_val; 
    /* Calculate f(x). */
    /* Store calculation in return_val. */ 
    return_val = x*x+2;
    return return_val; 
} /* m */


