#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int
rand_lim(int, int);

int **
populate(unsigned int, unsigned int);

int **
allocate(unsigned int, unsigned int);

void
print_matrix(unsigned int, unsigned int, int **);

void
cleanup(unsigned int, int **);

int **
calculate_simple(unsigned int, unsigned int, int **, int **);

typedef void (*benchmarkable)(unsigned int)(unsigned int)(int **)(int **);

void
benchmark(benchmarkable);

int
main(void)
{
    unsigned int n_line = 3;
    unsigned int n_column = 3;

    int **matrix_A = NULL;
    int **matrix_B = NULL;
    int **matrix_result = NULL;

    matrix_A = populate(n_line, n_column);
    matrix_B = populate(n_line, n_column);

    printf("\n\n-----------\nMATRIX A ");
    print_matrix(n_line, n_column, matrix_A);
    printf("\n\n-----------\nMATRIX B ");
    print_matrix(n_line, n_column, matrix_B);


    printf("\n\n-----------\nMATRIX RESULT ");
    print_matrix(n_line, n_column, matrix_result);

    cleanup(n_line, matrix_A);
    cleanup(n_line, matrix_B);
    cleanup(n_line, matrix_result);

    return 0;
}

int **
calculate_simple(unsigned int n_line, unsigned int n_column, int **matrix_A, int **matrix_B)
{

    int **matrix_result = NULL;
    matrix_result = allocate(n_line, n_column);

    unsigned int i;
    unsigned int j;
    unsigned int k;

    for (i = 0; i < n_line; i++)
        for (j = 0; j < n_column; j++)
            for (k = 0; k < n_line; k++)
                matrix_result[i][j] += matrix_A[i][k] * matrix_B[k][j];

    return matrix_result;
}

int **
populate(unsigned int n_line, unsigned int n_column)
{
    int **matrix = NULL;

    matrix = allocate(n_line, n_column);

    unsigned i;
    unsigned j;
    for (i = 0; i < n_line; ++i)
        for (j = 0; j < n_column; ++j)
            matrix[i][j] = rand_lim(2, 25);

    return matrix;
}
/*
 * Returns an integer in the range [from, to).
 */
int
rand_lim(int from, int to)
{
    int divisor = RAND_MAX / (to + 1);
    int toReturn;

    do
    {
        toReturn = rand() / divisor;
    }
    while (!(from <= toReturn && toReturn <= to));

    return toReturn;
}
void
print_matrix(unsigned int n_line, unsigned int n_column, int **matrix)
{
    printf("\n\n");

    unsigned int i;
    for (i = 0; i < n_line; ++i)
    {
        unsigned int j;
        for (j = 0; j < n_column; ++j)
        {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}
void
cleanup(unsigned int n_line, int **matrix)
{
    unsigned int i;
    for (i = 0; i < n_line; ++i)
        free(matrix[i]);
    free(matrix);
}
int **
allocate(unsigned int n_line, unsigned int n_column)
{
    int **matrix = NULL;

    matrix = (int **) calloc(n_line, sizeof(int **));

    unsigned i;
    for (i = 0; i < n_line; ++i)
        matrix[i] = (int *) calloc(n_column, sizeof(int *));

    return matrix;
}
int **
benchmark(benchmarkable function, unsigned int n_line, unsigned int n_column, int **matrix_A, int **matrix_B)
{
    int ** matrix_result = NULL;

    struct timeval tv;
    double start_t, end_t, tempo_gasto;

    gettimeofday(&tv, NULL);
    start_t = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;

    matrix_result = function(n_line, n_column, matrix_A, matrix_B);

    gettimeofday(&tv, NULL);
    end_t = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
    tempo_gasto = end_t - start_t;

    printf("Column order; tempo para multiplicar coluna: %f secs \n", tempo_gasto);

    return matrix_result;
}
