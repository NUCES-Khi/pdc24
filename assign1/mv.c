#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double **allocateMatrix(int rows, int cols) {
    double **matrix = (double **)malloc(rows * sizeof(double *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double *)malloc(cols * sizeof(double));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(1);
        }
    }
    return matrix;
}

void deallocateMatrix(double **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

void fillMatrixRandom(double **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = rand() / (double)RAND_MAX;
        }
    }
}

void printMatrix(double **matrix, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}

double *matrixVectorMultiplication(double **matrix, double *vector, int rows, int cols) {
    double *result = (double *)malloc(rows * sizeof(double));
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        result[i] = 0.0;
        for (int j = 0; j < cols; j++) {
            result[i] += matrix[i][j] * vector[j];
        }
    }
    return result;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Please provide two integers as command-line arguments.\n");
        return 1;
    }

    int matrixSize = atoi(argv[1]);
    int vectorSize = atoi(argv[2]);

    srand(time(NULL));

    double **matrix = allocateMatrix(matrixSize, vectorSize);
    double *vector = (double *)malloc(vectorSize * sizeof(double));
    if (vector == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    fillMatrixRandom(matrix, matrixSize, vectorSize);
    for (int i = 0; i < vectorSize; i++) {
        vector[i] = rand() / (double)RAND_MAX; 
    }

    double *result = matrixVectorMultiplication(matrix, vector, matrixSize, vectorSize);

    printf("Resultant vector:\n");
    for (int i = 0; i < matrixSize; i++) {
        printf("| %.6f |\n", result[i]);
    }

    deallocateMatrix(matrix, matrixSize);
    free(vector);
    free(result);

    return 0;
}
