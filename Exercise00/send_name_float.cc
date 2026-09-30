#include <cstdio>
#include <cstdlib>
#include <mpi.h>

int main(void)
{
    int rank, size;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char name[MPI_MAX_PROCESSOR_NAME];
    int len;
    MPI_Get_processor_name(name, &len);

    if (size < 4)
    {
        if (rank == 0)
            printf("This program requires at least 4 processes.\n");

        MPI_Finalize();
        return 0;
    }

    // Computer 4 = rank 3, Computer 2 = rank 1
    if (rank == 3)
    {
        float value = 25.5f;

        MPI_Ssend(name, MPI_MAX_PROCESSOR_NAME, MPI_CHAR,
                  1, 0, MPI_COMM_WORLD);

        MPI_Ssend(&value, 1, MPI_FLOAT,
                  1, 1, MPI_COMM_WORLD);

        printf("Computer 4 sent name %s and value %.2f to computer 2\n",
               name, value);
    }
    else if (rank == 1)
    {
        char receivedName[MPI_MAX_PROCESSOR_NAME];
        float receivedValue;

        MPI_Recv(receivedName, MPI_MAX_PROCESSOR_NAME, MPI_CHAR,
                 3, 0, MPI_COMM_WORLD, &status);

        MPI_Recv(&receivedValue, 1, MPI_FLOAT,
                 3, 1, MPI_COMM_WORLD, &status);

        printf("Computer 2 received computer name: %s\n", receivedName);
        printf("Computer 2 received floating point value: %.2f\n",
               receivedValue);
    }

    MPI_Finalize();
    return 0;
}
