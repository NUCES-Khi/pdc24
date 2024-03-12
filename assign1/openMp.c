#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

double** allocate_matrix(int rows, int cols) {
    double** mat = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        mat[i] = (double*)malloc(cols * sizeof(double));
    }
    return mat;
}

double* allocate_vector(int size) {
    double* vec = (double*)malloc(size * sizeof(double));
    return vec;
}

void initialize(double** mat, double* vec, int rows, int cols) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat[i][j] = (double)rand() / RAND_MAX;
        }
    }
    #pragma omp parallel for
    for (int i = 0; i < cols; i++) {
        vec[i] = (double)rand() / RAND_MAX;
    }
}

void matvec_multiply(double** mat, double* vec, double* result, int rows, int cols) {
    #pragma omp parallel for
    for (int i = 0; i < rows; i++) {
        result[i] = 0.0;
        for (int j = 0; j < cols; j++) {
            result[i] += mat[i][j] * vec[j];
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <matrix_size> <vector_size>\n", argv[0]);
        return 1;
    }

    int matrix_size = atoi(argv[1]);
    int vector_size = atoi(argv[2]);

    double** matrix = allocate_matrix(matrix_size, vector_size);
    double* vector = allocate_vector(vector_size);
    double* result = allocate_vector(matrix_size);

    initialize(matrix, vector, matrix_size, vector_size);

    matvec_multiply(matrix, vector, result, matrix_size, vector_size);

    for (int i = 0; i < matrix_size; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(result);

    return 0;
}


