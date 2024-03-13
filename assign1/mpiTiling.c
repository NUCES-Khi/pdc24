#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define TILE_SIZE 16

double* allocate_vector(int size) {
    double* vector = (double*)malloc(size * sizeof(double));
    return vector;
}

double** allocate_matrix(int rows, int columns) {
    double** matrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double*)malloc(columns * sizeof(double));
    }
    return matrix;
}

void initialize(double** matrix, double* vector, int rows, int columns) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
    }
    for (int i = 0; i < columns; i++) {
        vector[i] = (double)rand() / RAND_MAX;
    }
}

void matvec_multiply(double** matrix, double* vector, double* result, int rows, int columns, int local_rows, int my_rank) {
    for (int i = 0; i < rows; i += TILE_SIZE) {
        for (int j = 0; j < columns; j++) {
            double temp = 0.0;
            for (int k = i; k < i + TILE_SIZE && k < rows; k++) {
                temp += matrix[k][j] * vector[k];
            }
            MPI_Reduce(&temp, &result[i + j], 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
        }
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 3) {
        if (rank == 0) {
            fprintf(stderr, "Usage: %s <matrix_size> <vector_size>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    int matrix_size = atoi(argv[1]);
    int vector_size = atoi(argv[2]);

    double* vector = NULL;
    double** matrix = NULL;
    double* result = NULL;

    if (rank == 0) {
        matrix = allocate_matrix(matrix_size, vector_size);
        vector = allocate_vector(vector_size);
        result = allocate_vector(matrix_size);

        initialize(matrix, vector, matrix_size, vector_size);
    }

    MPI_Bcast(vector, vector_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int local_rows = matrix_size / size;
    double** local_matrix = allocate_matrix(local_rows, vector_size);
    double* local_result = allocate_vector(local_rows);

    MPI_Scatter(matrix[0], local_rows * vector_size, MPI_DOUBLE, local_matrix[0], local_rows * vector_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    matvec_multiply(local_matrix, vector, local_result, matrix_size, vector_size, local_rows, rank);

    if (rank == 0) {
        // Gather results from all processes
        MPI_Gather(local_result, local_rows, MPI_DOUBLE, result, local_rows, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Print the resulting vector in matrix form
        for (int i = 0; i < matrix_size; i++) {
            printf("%.2f ", result[i]);
            if ((i + 1) % vector_size == 0) {
                printf("\n");
            }
        }

        // Free allocated memory
        free(vector);
        free(result);
        for (int i = 0; i < matrix_size; i++) {
            free(matrix[i]);
        }
        free(matrix);
    }

    // Free local memory
    free(local_matrix);
    free(local_result);

    MPI_Finalize();

    return 0;
}
