#include <iostream>
#include <random>
#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const long long TOTAL_POINTS = 10000000;

    long long base = TOTAL_POINTS / size;
    long long remainder = TOTAL_POINTS % size;

    long long localPoints =
        base + (rank < remainder ? 1 : 0);

    // Different random-number sequence for each MPI process
    std::mt19937_64 generator(12345 + rank);
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    MPI_Barrier(MPI_COMM_WORLD);
    double startTime = MPI_Wtime();

    long long localInside = 0;

    for (long long i = 0; i < localPoints; i++)
    {
        double x = distribution(generator);
        double y = distribution(generator);

        if ((x * x + y * y) <= 1.0)
        {
            localInside++;
        }
    }

    long long totalInside = 0;

    MPI_Reduce(
        &localInside,
        &totalInside,
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
        double pi =
            4.0 * static_cast<double>(totalInside) /
            static_cast<double>(TOTAL_POINTS);

        std::cout << "Number of processes: " << size << std::endl;
        std::cout << "Number of points: " << TOTAL_POINTS << std::endl;
        std::cout << "Estimated Pi: " << pi << std::endl;
        std::cout << "Execution time: " << totalTime << " seconds" << std::endl;
    }

    MPI_Finalize();
    return 0;
}
