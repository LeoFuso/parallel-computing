#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <stdlib.h>

#ifdef _OPENMP
#include <omp.h>
#endif

#include <omp.h>
#include <stdint.h>

#define LIMIT 5000000

unsigned int
is_prime(uint32_t number)
{
    uint32_t upper_lmt, i = 0;
    upper_lmt = (uint32_t) sqrt(number);
    for (i = 3; i <= upper_lmt; i += 2)
    {
        if (number % i == 0)
        {
            return 0;
        }
    }

    return 1;
}

int
main(void)
{
    /* if OpenMP is undefined */
    unsigned int thread_count = 1;

    /* 2 is prime, we'll start with that */
    uint32_t prime_count = 1;

#ifdef _OPENMP
    thread_count = omp_get_max_threads();
#endif

    struct timeval tv;
    double start_t, end_t, runtime;

    gettimeofday(&tv, NULL);
    start_t = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;

    uint32_t i;
#pragma omp parallel for num_threads(thread_count) reduction(+: prime_count)
    for (i = 3; i < LIMIT; i += 2)
    {
        unsigned int p = is_prime(i);
        prime_count += p;
    }

    gettimeofday(&tv, NULL);
    end_t = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
    runtime = end_t - start_t;

    printf("QUANTIDADE DE THREADS USADAS: %d\n", thread_count);
    printf("TOTAL NÚMEROS PRIMOS ATÉ %d: %d\n", LIMIT, prime_count);
    printf("TEMPO GASTO: %f\n", runtime);
}