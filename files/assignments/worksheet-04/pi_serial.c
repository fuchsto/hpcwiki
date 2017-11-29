#include <mpi.h>
#include <time.h>   // seeding prng
#include <math.h>   // sqrt
#include <stdio.h>  // printf
#include <stdlib.h> // srand, rand

void monte_carlo_pi(int iters, int * hits, int * misses)
{
    int i, rank;
    double x, y;

    // initialize (crappy) pseudo random number generator:
    srand(time(NULL)i * rank + 1);

    // initialize counters

    *hits   = 0;
    *misses = 0;

    ???
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

    // measurement start time:
    time = -MPI_Wtime();

    monte_carlo_pi(iters, &hits, &misses);

    time += MPI_Wtime();

    pi = 4 * (((double) hits) / iters);
    printf("Estimate after %i iterations = %f\n", iters, pi);
    printf("This took %f seconds with %i procs\n", time, size);

    return 0;
}

