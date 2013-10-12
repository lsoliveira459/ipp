#include <stdio.h>
#include "mpi.h"

/* Constroi o tipo derivado */
void Build_derived_type(
         float*         a_ptr           /* entrada */,
         float*         b_ptr           /* entrada */,
         int*           n_ptr           /* entrada */,
         MPI_Datatype*  mesg_mpi_t_ptr  /* saida   */);

/* Recebe os valores de a, b e n e envia para todos os processos */
void Get_data3(
        float*          a_ptr           /* saida   */, 
        float*          b_ptr           /* saida   */, 
        int*            n_ptr           /* saida   */, 
        int             my_rank         /* entrada */);

/* Calcula o trapezoide local */
float Trap(
        float           local_a         /* entrada */,
        float           local_b         /* entrada */,
        int             local_n         /* entrada */,
        float           h               /* entrada */)/

int main(int argc, char** argv) {
    int         r, p, n, local_n, source, dest, tag;
    float       a, b, h, local_a, local_b, integral, total;
    MPI_Status  status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &r);
    MPI_Comm_size(MPI_COMM_WORLD, &p);

    Get_data3(&a, &b, &n, r);

    h = (b-a)/n;
    local_n = n/p;

    local_a = a + r*local_n*h;
    local_b = local_a + local_n*h;
    integral = Trap(local_a, local_b, local_n, h);

    MPI_Reduce(&integral, &total, 1, MPI_FLOAT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (my_rank == 0) {
        printf("With n = %d trapezoids, our estimate\n", n);
        printf("of the integral from %f to %f = %f\n", a, b, total); 
    }

    MPI_Finalize();
}

void Build_derived_type(float* a_ptr, float* b_ptr, int* n_ptr, MPI_Datatype* mesg_mpi_t_ptr) {
    int block_lengths[3];      
    MPI_Aint displacements[3], start_address, address;
    MPI_Datatype typelist[3];  

    /* Definindo o numero de elementos de cada bloco. */
    block_lengths[0] = block_lengths[1] = block_lengths[2] = 1;

    /* Definicao dos tipos que serao usados */
    typelist[0] = MPI_FLOAT;
    typelist[1] = MPI_FLOAT;
    typelist[2] = MPI_INT;

    /* Definindo a distancia do primeiro elemento como 0.
     * Em seguida calcula-se a posicao de 'a' e de todos os
     * elementos com relacao a 'a'. */
    displacements[0] = 0;
    MPI_Address(a_ptr, &start_address);
    MPI_Address(b_ptr, &address);
    displacements[1] = address - start_address;
    MPI_Address(n_ptr, &address);
    displacements[2] = address - start_address;

    /* Construcao do tipo derivado. */
    MPI_Type_struct(3, block_lengths, displacements, typelist, mesg_mpi_t_ptr);

    /* Indicando para o sistema que este tipo derivado sera usado */
    MPI_Type_commit(mesg_mpi_t_ptr);
}

void Get_data3(float* a_ptr, float* b_ptr, int* n_ptr, int my_rank) {
    MPI_Datatype  mesg_mpi_t;
    
    if (my_rank == 0){
        printf("Enter a, b, and n\n");
        scanf("%f %f %d", a_ptr, b_ptr, n_ptr);
    }

    Build_derived_type(a_ptr, b_ptr, n_ptr, &mesg_mpi_t);
    MPI_Bcast(a_ptr, 1, mesg_mpi_t, 0, MPI_COMM_WORLD);
}

float Trap(float local_a, float local_b, int local_n, float h) { 
    float x, integral;
    int i; 

    /* Prototipo da funcao a ser integrada */
    float f(float x);

    integral = (f(local_a) + f(local_b))/2.0; 
    x = local_a; 
    for (i = 1; i <= local_n-1; i++) { 
        x = x + h; 
        integral = integral + f(x); 
    } 
    integral = integral*h; 
    return integral;
}

float f(float x) { 
    float return_val; 
    return_val = x*x;
    return return_val; 
}
