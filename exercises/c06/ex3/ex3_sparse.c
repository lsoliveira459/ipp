#include "mpi.h"

typedef struct {
    int x;
    int y;
    float v;
} sparce_matrix;

void build_derived_type(sparce_matrix* mat,MPI_Datatype* msg_mpi_t_ptr) {
    int block_lengths[] = {1,1,1};
    MPI_Aint displacements[3], start_address, address;
    MPI_Datatype typelist[3];

    typelist[0] = MPI_INT;
    typelist[1] = MPI_INT;
    typelist[2] = MPI_FLOAT;

    displacements[0] = 0;
    MPI_Address(&(mat->x), &start_address);
    MPI_Address(&(mat->y), &address);
    displacements[1] = address - start_address;
    MPI_Address(&(mat->v), &address);
    displacements[2] = address - start_address;

    MPI_Type_struct(3, block_lengths, displacements, typelist, msg_mpi_t_ptr);
    MPI_Type_commit(msg_mpi_t_ptr);
}
