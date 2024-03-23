/*
 * Programmer(s) : Muneer Raza
 * Date: 13/03/2024
 * Desc: Parallel version of matrix vector multiplication using MPI library.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

// Function to initialize matrix and vector with random values
void initialize_random_values(double *matrix, double *vector, int size) {
    // Seed the random number generator
    srand(time(NULL));

    // Initialize matrix with random values
    for (int i = 0; i < size * size; i++) {
        matrix[i] = (double)rand() / RAND_MAX; // Random value between 0 and 1
    }

    // Initialize vector with random values
    for (int i = 0; i < size; i++) {
        vector[i] = (double)rand() / RAND_MAX; // Random value between 0 and 1
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
    double *result = malloc(size * sizeof(double));

    // Initialize MPI
    MPI_Init(&argc, &argv);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Initialize matrix and vector with random values
    if (world_rank == 0) {
        initialize_random_values(matrix, vector, size);
    }

    // Broadcast the vector to all processes
    MPI_Bcast(vector, size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Scatter the matrix rows
    double *sub_matrix = malloc(size * (size / world_size) * sizeof(double));
    MPI_Scatter(matrix, size * (size / world_size), MPI_DOUBLE, sub_matrix, size * (size / world_size), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Perform the multiplication
    double start = MPI_Wtime();
    for (int i = 0; i < size / world_size; i++) {
        result[i] = 0;
        for (int j = 0; j < size; j++) {
            result[i] += sub_matrix[i * size + j] * vector[j];
        }
    }
    double end = MPI_Wtime();

    // Gather the results
    double *final_result = NULL;
    if (world_rank == 0) {
        final_result = malloc(size * sizeof(double));
    }
    MPI_Gather(result, size / world_size, MPI_DOUBLE, final_result, size / world_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Print time taken
    if (world_rank == 0) {
        printf("Time taken: %f seconds\n", end - start);
        free(final_result);
    }

    // Clean up
    free(matrix);
    free(vector);
    free(result);
    free(sub_matrix);

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
    