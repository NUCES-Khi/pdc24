/*
 * Programmer(s) : Muneer Raza
 * Date: 13/03/2024
 * Desc: Parallel version of matrix vector multiplication using the OPEN MP library with tiling. 
 */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

#define TILE_SIZE 16 // Define the tile size

void matrix_vector_multiplication(double *matrix, double *vector, double *result, int size) {
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < size; i += TILE_SIZE) {
        for (int j = 0; j < size; j += TILE_SIZE) {
            for (int k = i; k < i + TILE_SIZE; ++k) {
                double temp = 0.0;
                for (int l = j; l < j + TILE_SIZE; ++l) {
                    temp += matrix[k * size + l] * vector[l];
                }
                #pragma omp atomic
                result[k] += temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Matrix size not defined\n");
        exit(1);
    }

    int size = atoi(argv[1]);
    double *matrix = malloc(size * size * sizeof(double));
    double *vector = malloc(size * sizeof(double));
    double *result = calloc(size, sizeof(double)); // Initialize result array to 0

    // Seed the random number generator
    srand(time(NULL));

    // Initialize matrix and vector with random values
    for (int i = 0; i < size * size; i++) {
        matrix[i] = rand() / (double)RAND_MAX;
    }
    for (int i = 0; i < size; i++) {
        vector[i] = rand() / (double)RAND_MAX;
    }

    // Perform matrix-vector multiplication
    double start_time = omp_get_wtime();
    matrix_vector_multiplication(matrix, vector, result, size);
    double end_time = omp_get_wtime();

    // Print time taken
    printf("Time taken: %f seconds\n", end_time - start_time);

    // Clean up
    free(matrix);
    free(vector);
    free(result);

    return 0;
}
 