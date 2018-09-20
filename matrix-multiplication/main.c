#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

int
rand_lim(int, int);

int **
populate(unsigned int);

int **
allocate(unsigned int);

unsigned int
check_sum(unsigned int, int **);

void
cleanup(unsigned int, int **);

double
benchmark(int **(*fn)(unsigned int, int **, int **, int **),
          unsigned int,
          int **,
          int **);

int **
calculate_simple(unsigned int, int **, int **, int **);

int **
calculate_swapped(unsigned int, int **, int **, int **);

int **
calculate_in_blocks(unsigned int, int **, int **, int **);

int
main(void)
{
	unsigned int size = 2048;

	int **matrix_A = NULL;
	int **matrix_B = NULL;

	int **
	(*calculate_v1)(unsigned int, int **, int **, int **) = NULL;

	int **
	(*calculate_v2)(unsigned int, int **, int **, int **) = NULL;

	int **
	(*calculate_v3)(unsigned int, int **, int **, int **) = NULL;

	matrix_A = populate(size);
	matrix_B = populate(size);

	calculate_v1 = calculate_simple;
	calculate_v2 = calculate_swapped;
	calculate_v3 = calculate_in_blocks;

	printf("CALCULATE CLASSIC\n");
	benchmark(calculate_v1, size, matrix_A, matrix_B);
	printf("CALCULATE REVERSE\n");
	benchmark(calculate_v2, size, matrix_A, matrix_B);
	printf("CALCULATE IN BLOCKS\n");
	benchmark(calculate_v3, size, matrix_A, matrix_B);

	cleanup(size, matrix_A);
	cleanup(size, matrix_B);

	return 0;
}

int **
calculate_simple(unsigned int size, int **matrix_A, int **matrix_B, int **matrix_result)
{
	unsigned int i;
	unsigned int j;
	unsigned int k;

	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			for (k = 0; k < size; k++)
				matrix_result[i][j] += matrix_A[i][k] * matrix_B[k][j];

	return matrix_result;
}

int **
calculate_swapped(unsigned int size, int **matrix_A, int **matrix_B, int **matrix_result)
{
	unsigned int i;
	unsigned int j;
	unsigned int k;

	for (i = 0; i < size; i++)
		for (k = 0; k < size; k++)
			for (j = 0; j < size; j++)
				matrix_result[i][j] += matrix_A[i][k] * matrix_B[k][j];
	return matrix_result;
}

int **
calculate_in_blocks(unsigned int size, int **matrix_A, int **matrix_B, int **matrix_result)
{
	unsigned int i;
	unsigned int j;
	unsigned int k;

	unsigned int ii;
	unsigned int jj;
	unsigned int kk;

	unsigned int block_size = 512;

	for (i = 0; i < size; i += block_size)
		for (k = 0; k < size; k += block_size)
			for (j = 0; j < size; j += block_size)
				for (ii = i; ii < i + block_size; ii++)
					for (kk = k; kk < k + block_size; kk++)
						for (jj = j; jj < j + block_size; jj++)
							matrix_result[ii][jj] += matrix_A[ii][kk] * matrix_B[kk][jj];

	return matrix_result;
}

double
benchmark(int **(*fn)(unsigned int, int **, int **, int **),
          unsigned int size,
          int **matrix_A,
          int **matrix_B)
{
	int **matrix_result = NULL;
	matrix_result = allocate(size);

	struct timeval tv;
	double start_t, end_t, runtime;

	gettimeofday(&tv, NULL);
	start_t = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;

	matrix_result = fn(size, matrix_A, matrix_B, matrix_result);

	gettimeofday(&tv, NULL);
	end_t = (double) tv.tv_sec + (double) tv.tv_usec / 1000000.0;
	runtime = end_t - start_t;

	printf("  -   RUNTIME: %f seconds\n", runtime);

	printf("  -    RESULT: %u\n", check_sum(size, matrix_result));
	cleanup(size, matrix_result);

	return runtime;
}

int **
populate(unsigned int size)
{
	int **matrix = NULL;

	matrix = allocate(size);

	unsigned i;
	unsigned j;
	for (i = 0; i < size; ++i)
		for (j = 0; j < size; ++j)
			matrix[i][j] = rand_lim(2, 25);

	return matrix;
}

int **
allocate(unsigned int size)
{
	int **matrix = NULL;

	matrix = (int **) calloc(size, sizeof(int **));

	unsigned i;
	for (i = 0; i < size; ++i)
		matrix[i] = (int *) calloc(size, sizeof(int *));

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
cleanup(unsigned int n_line, int **matrix)
{
	unsigned int i;
	for (i = 0; i < n_line; ++i)
		free(matrix[i]);
	free(matrix);
}

unsigned int
check_sum(unsigned int size, int **matrix)
{
	unsigned int i;
	unsigned int j;

	unsigned int result = 0;

	for (i = 0; i < size; ++i)
		for (j = 0; j < size; ++j)
			result += matrix[i][j];
	return result;
}