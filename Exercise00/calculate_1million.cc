#include <cstdio>
#include <cstdlib>
#include <mpi.h>
#define N 1000000

int main(void)
{
    int rank, size;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;

    int worksize = N / size;

    int start = rank * worksize;
    int end = start + worksize - 1;

    long long sum = 0;
    for (int r = start; r < end; r++)
        sum = sum + r;

    if (rank == 0)
    {
        long long total = sum;
        long long mysum;

        for (int r = 1; r < size; r++)
        {
            MPI_Recv(&mysum, 1, MPI_LONG_LONG, r, 0,
                     MPI_COMM_WORLD, &status);
            total += mysum;
        }

        printf("Total is %lld\n", total);
    }
    else
    {
        MPI_Ssend(&sum, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}
