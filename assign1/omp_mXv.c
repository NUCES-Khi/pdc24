/*
 * Programmer(s) : Muneer Raza
 * Date: 12/03/2024
 * Desc: Parallel version of matrix vector multiplication using OPEN MP library. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

// Function to allocate memory for a matrix
double **allocate_matrix(int rows, int cols) {
    double **matrix = (double **)malloc(rows * sizeof(double *));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double *)malloc(cols * sizeof(double));
    }
    return matrix;
}

// Function to free memory allocated for a matrix
void free_matrix(double **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to generate random values for a matrix and a vector
void generate_random_values(double **matrix, double *vector, int size) {
    srand(42); // Seed for reproducibility
    for (int i = 0; i < size; i++) {
        vector[i] = (double)rand() / RAND_MAX; // Random value between 0 and 1
        for (int j = 0; j < size; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX; // Random value between 0 and 1
        }
    }
}

// Function to perform matrix-vector multiplication in parallel
void matrix_vector_multiply_parallel(double **matrix, double *vector, double *result, int size) {
    #pragma omp parallel for
    for (int i = 0; i < size; i++) {
        result[i] = 0.0;
        for (int j = 0; j < size; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Matrix size not defined\n");
        return 1;
    }

    int size = atoi(argv[1]);

    // Allocate memory for the matrix and vector
    double **matrix = allocate_matrix(size, size);
    double *vector = (double *)malloc(size * sizeof(double));
    double *result = (double *)malloc(size * sizeof(double));

    // Generate random values for the matrix and vector
    generate_random_values(matrix, vector, size);

    // Perform matrix-vector multiplication in parallel and measure the time taken
    double start = omp_get_wtime();
    matrix_vector_multiply_parallel(matrix, vector, result, size);
    double end = omp_get_wtime();

    printf("Time taken: %f seconds\n", end - start);

    free_matrix(matrix, size);
    free(vector);
    free(result);

    return 0;
}
