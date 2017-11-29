#include <mpi.h>
#include <time.h>   // seeding prng
#include <math.h>   // sqrt
#include <stdio.h>  // printf
#include <stdlib.h> // srand, rand

void monte_carlo_pi(int iters, int * hits, int * misses)
{
    int i, rank;
    double x, y;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // initialize (crappy) pseudo random number generator:
    srand(time(NULL)i * rank + 1);

    // initialize counters

    *hits   = 0;
    *misses = 0;

    // < ??? >
}

int main(int argc, char** argv)
{
    double pi;

    int hits;
    int misses;
    int iters = 100000000; // 100 mio.

    double time;

    int rank;
    int size;
    int local_iters;
    int local_hits;
    int local_misses;

    MPI_Init(&argc, &argv);

    // sync for fair timing:
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // how many iterations we need in each process:
    local_iters = iters / size;

    // remaining iterations:
    if (rank == 0) local_iters = iters - (local_iters * (size-1));

    // measurement start time:
    time = -MPI_Wtime();

    monte_carlo_pi(local_iters, &local_hits, &local_misses);

    // reduce hits on rank 0
    MPI_Reduce(/* < ???> */);

    time += MPI_Wtime();

    if (rank == 0) {
        pi = 4 * (((double) hits) / iters);
        printf("Estimate after %i iterations = %f\n", iters, pi);
        printf("This took %f seconds with %i procs\n", time, size);
    }

    MPI_Finalize();

    return 0;
}

