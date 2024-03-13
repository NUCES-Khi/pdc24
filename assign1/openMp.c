#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double** allocate_matrix(int rows, int cols) {
    double** matrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*)malloc(cols * sizeof(double));
    }
    return matrix;
}

double* allocate_vector(int size) {
    double* vector = (double*)malloc(size * sizeof(double));
    return vector;
}

void initialize(double** matrix, double* vector, int rows, int cols) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
    }
    #pragma omp parallel for
    for (int i = 0; i < cols; i++) {
        vector[i] = (double)rand() / RAND_MAX;
    }
}

void matrix_vector_multiply(double** matrix, double* vector, double* result, int rows, int cols) {
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        result[i] = 0.0;
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Please provide two integers as command-line arguments.\n");
        return 1;
    }

    int matrix_rows = atoi(argv[1]);
    int vector_size = atoi(argv[2]);

    double** matrix = allocate_matrix(matrix_rows, vector_size);
    double* vector = allocate_vector(vector_size);
    double* result = allocate_vector(matrix_rows);

    initialize(matrix, vector, matrix_rows, vector_size);

    matrix_vector_multiply(matrix, vector, result, matrix_rows, vector_size);

    // Print the result in matrix form
    printf("Result of matrix-vector multiplication:\n");
    for (int i = 0; i < matrix_rows; i++) {
        printf("| %f |\n", result[i]);
    }

    // Free allocated memory
    for (int i = 0; i < matrix_rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);

    return 0;
}
