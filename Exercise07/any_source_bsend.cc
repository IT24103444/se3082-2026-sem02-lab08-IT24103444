#include <cstdio>
#include <cstdlib>
#include <mpi.h>

int main(void)
{
    int rank;
    MPI_Status status;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    char name[30];
    int len;
    MPI_Get_processor_name(name, &len);

    int x = 30;
    int y;

    // Create and attach buffer required by MPI_Bsend
    int bufferSize = sizeof(int) + MPI_BSEND_OVERHEAD;
    char *buffer = new char[bufferSize];
    MPI_Buffer_attach(buffer, bufferSize);

    if (rank == 1)
    {
        printf("Sending buffered message to computer 3 from computer 1\n");

        MPI_Bsend(
            &x,
            1,
            MPI_INT,
            3,
            0,
            MPI_COMM_WORLD
        );
    }
    else if (rank == 3)
    {
        MPI_Recv(
            &y,
            1,
            MPI_INT,
            MPI_ANY_SOURCE,
            0,
            MPI_COMM_WORLD,
            &status
        );

        printf("In computer 3 the value of y is %d\n", y);
        printf("Message received from rank %d\n", status.MPI_SOURCE);
    }
    else
    {
        printf(
            "Just a normal process From rank %d machine %s\n",
            rank,
            name
        );
    }

    void *detachedBuffer;
    int detachedSize;

    MPI_Buffer_detach(&detachedBuffer, &detachedSize);
    delete[] buffer;

    MPI_Finalize();
    return 0;
}
