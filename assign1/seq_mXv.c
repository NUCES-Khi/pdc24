/*
 * Programmer(s) : Muneer Raza
 * Date: 12/03/2024
 * Desc: Sequential version of matrix vector multiplication. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to allocate memory for a matrix
double **allocate_matrix(int rows, int cols) {
    double **matrix = (double **)malloc(rows * sizeof(double *)); // Allocate memory for the rows
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double *)malloc(cols * sizeof(double)); // Allocate memory for the columns
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
    srand(time(NULL));
    for (int i = 0; i < size; i++) {
        
        vector[i] = (double)rand() / RAND_MAX; // Random value between 0 and 1
        for (int j = 0; j < size; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX; // Random value between 0 and 1
        }
    }
}

// Function to perform matrix-vector multiplication
void matrix_vector_multiply(double **matrix, double *vector, double *result, int size) {
    for (int i = 0; i < size; i++) {
        result[i] = 0.0;
        for (int j = 0; j < size; j++) {
            result[i] += matrix[i][j] * vector[j]; // Multiply each element of the row with the corresponding element of the vector and add the results
        }
    }
}

int main(int argc, char *argv[]) { // Command line arguments: matrix/vector size
    if (argc != 2) {
        fprintf(stderr, "Matrix size not defined\n");
        return 1;
    }

    int size = atoi(argv[1]); // Size of the matrix and vector

    double **matrix = allocate_matrix(size, size); // Allocate memory for the matrix
    double *vector = (double *)malloc(size * sizeof(double)); // Allocate memory for the vector
    double *result = (double *)malloc(size * sizeof(double)); // Allocate memory for the result

    generate_random_values(matrix, vector, size);

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    matrix_vector_multiply(matrix, vector, result, size);
    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_spent = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("Time taken: %f seconds\n", time_spent);

    free_matrix(matrix, size);
    free(vector);
    free(result);

    return 0;
}
