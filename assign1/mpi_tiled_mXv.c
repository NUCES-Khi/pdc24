/*
 * Programmer(s) : Muneer Raza
 * Date: 13/03/2024
 * Desc: Parallel version of matrix vector multiplication using the MPI library with tiling.  
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define TILE_SIZE 32 // Define the size of each tile/block

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

    // Calculate the number of tiles per row/column
    int num_tiles = size / TILE_SIZE;
    if (size % TILE_SIZE != 0) {
        num_tiles++; // Include a partial tile if necessary
    }

    // Allocate memory for the sub-matrix tile
    double *sub_matrix_tile = malloc(TILE_SIZE * TILE_SIZE * sizeof(double));

    // Perform tiling and multiplication
    double start = MPI_Wtime();
    for (int tile_row = 0; tile_row < num_tiles; tile_row++) {
        for (int tile_col = 0; tile_col < num_tiles; tile_col++) {
            // Calculate the starting indices of the current tile
            int start_row = tile_row * TILE_SIZE;
            int start_col = tile_col * TILE_SIZE;

            // Load the sub-matrix tile
            for (int i = 0; i < TILE_SIZE; i++) {
                for (int j = 0; j < TILE_SIZE; j++) {
                    int global_row = start_row + i;
                    int global_col = start_col + j;
                    if (global_row < size && global_col < size) {
                        sub_matrix_tile[i * TILE_SIZE + j] = matrix[global_row * size + global_col];
                    } else {
                        sub_matrix_tile[i * TILE_SIZE + j] = 0.0; // Pad with zeros for out-of-bound elements
                    }
                }
            }

            // Perform local multiplication
            double partial_result = 0.0;
            for (int i = 0; i < TILE_SIZE; i++) {
                for (int j = 0; j < TILE_SIZE; j++) {
                    partial_result += sub_matrix_tile[i * TILE_SIZE + j] * vector[start_col + j];
                }
            }

            // Reduce the partial results
            double total_result;
            MPI_Allreduce(&partial_result, &total_result, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);

            // Store the result in the global result array
            if (world_rank == 0) {
                int global_row = start_row;
                result[global_row] = total_result;
            }
        }
    }
    double end = MPI_Wtime();

    // Print time taken
    if (world_rank == 0) {
        printf("Time taken: %f seconds\n", end - start);
    }

    // Clean up
    free(matrix);
    free(vector);
    free(result);
    free(sub_matrix_tile);

    // Finalize MPI
    MPI_Finalize();

    return 0;
}
