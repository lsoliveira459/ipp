#include <stdio.h>
#include "mpi.h"

main(int argc, char** argv) {
    int         my_rank;   
    int         p;         
    float       a;         
    float       b;         
    int         n;         
    int         f;         
    float       h;         
    float       local_a;   
    float       local_b;   
    int         local_n;   
                           
    float     (*local_f)(float); 
    float       integral;  
    float       total;     
    int         source;    
    int         dest = 0;  
    int         tag = 0;
    MPI_Status  status;

    void Get_data(float* a_ptr, float* b_ptr, int* n_ptr, int* f_ptr, int my_rank, int p);
    float Trap(float local_a, float local_b, int local_n, float (*f)(float), float h);    

    
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

    h = (b-a)/n;    
    local_n = n/p;  


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
void Get_data(float* a_ptr, float* b_ptr, int* n_ptr,int* f_ptr, int my_rank, int p) {
    int source = 0;    
    int dest;          
    int tag;
    MPI_Status status;

    if (my_rank == 0){
		printf("Functions (f):\n0. x**2\n1. x**2+1\n2. x**2+2")
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
}
float Trap(float local_a, float local_b, int local_n, float (*local_f)(float), float h) { 
    float integral;    
    float x; 
    int i;

    integral = ((*local_f)(local_a) + (*local_f)(local_b))/2.0; 
    x = local_a; 
    for (i = 1; i <= local_n-1; i++) { 
        x = x + h; 
        integral = integral + (*local_f)(x); 
    } 
    integral = integral*h; 
    return integral;
} 
float k(float x) { 
    float return_val; 
    
     
    return_val = x*x;
    return return_val; 
} 
float l(float x) { 
    float return_val; 
    return_val = x*x+1;
    return return_val; 
} 
float m(float x) { 
    float return_val; 
    return_val = x*x+2;
    return return_val; 
}