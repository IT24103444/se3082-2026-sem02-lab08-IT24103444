#include <iostream>
#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long N = 10000000;

    // Divide the numbers among MPI processes
    long long base = N / size;
    long long remainder = N % size;

    long long count = base + (rank < remainder ? 1 : 0);
    long long start = rank * base + (rank < remainder ? rank : remainder) + 1;
    long long end = start + count - 1;

    MPI_Barrier(MPI_COMM_WORLD);
    double startTime = MPI_Wtime();

    long long localSum = 0;

    for (long long i = start; i <= end; i++)
    {
        localSum += i;
    }

    long long totalSum = 0;

    MPI_Reduce(
        &localSum,
        &totalSum,
        1,
        MPI_LONG_LONG,
        MPI_SUM,
        0,
        MPI_COMM_WORLD
    );

    double localTime = MPI_Wtime() - startTime;
    double totalTime = 0.0;

    MPI_Reduce(
        &localTime,
        &totalTime,
        1,
        MPI_DOUBLE,
        MPI_MAX,
        0,
        MPI_COMM_WORLD
    );

    if (rank == 0)
    {
        std::cout << "Number of processes: " << size << std::endl;
        std::cout << "Sum from 1 to " << N << " = " << totalSum << std::endl;
        std::cout << "Execution time: " << totalTime << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
