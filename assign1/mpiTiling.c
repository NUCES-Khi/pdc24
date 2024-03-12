
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TILE_SIZE 16

double** allocate_matrix(int rows, int columns) {
    double** matrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*)malloc(columns * sizeof(double));
    }
    return matrix;
}

double* allocate_vector(int size) {
    double* vector = (double*)malloc(size * sizeof(double));
    return vector;
}

void initialize(double** matrix, double* vector, int rows, int columns) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
    }
    #pragma omp parallel for
    for (int i = 0; i < columns; i++) {
        vector[i] = (double)rand() / RAND_MAX;
    }
}

void matvec_multiply(double** matrix, double* vector, double* result, int rows, int columns) {
    #pragma omp parallel for
    for (int i = 0; i < rows; i += TILE_SIZE) {
        for (int j = 0; j < columns; j++) {
            double temp = 0.0;
            for (int k = i; k < i + TILE_SIZE && k < rows; k++) {
                temp += matrix[k][j] * vector[k];
            }
            #pragma omp atomic
            result[i + j] += temp;
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
