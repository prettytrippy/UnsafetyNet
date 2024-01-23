#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include "matrix.h"

matrix *create_value_matrix(long rows, long cols, double value) {
    matrix *m = malloc(sizeof(matrix));
    m->cols = cols;
    m->rows = rows;
    m->data = (double *)malloc(sizeof(double) * rows * cols);

    for (long i = 0; i < rows * cols; i++) {
        m->data[i] = value;
    }
    return m;
}

matrix *create_random_matrix(long rows, long cols) {
    matrix *m = malloc(sizeof(matrix));
    m->cols = cols;
    m->rows = rows;
    m->data = (double *)malloc(sizeof(double) * rows * cols);

    for (long i = 0; i < rows * cols; i++) {
        m->data[i] = (((double)rand() * 2.0 / (double)RAND_MAX) - 1.0);
    }
    return m;
}

matrix *create_He_initialized_matrix(long rows, long cols) {
    matrix *m = malloc(sizeof(matrix));
    m->cols = cols;
    m->rows = rows;
    m->data = (double *)malloc(sizeof(double) * rows * cols);

    // Calculate the variance for He initialization
    double variance = 2.0 / (double)cols; // 'cols' is the number of input units

    for (long i = 0; i < rows * cols; i++) {
        // Generate a random number from a Gaussian distribution with mean 0 and variance 'variance'
        double random_value = sqrt(variance) * (((double)rand() / RAND_MAX) * 2.0 - 1.0);
        m->data[i] = random_value;
    }
    return m;
}

matrix *create_scaled_random_matrix(long rows, long cols, double scale) {
    matrix *m = malloc(sizeof(matrix));
    m->cols = cols;
    m->rows = rows;
    m->data = (double *)malloc(sizeof(double) * rows * cols);

    for (long i = 0; i < rows * cols; i++) {
        m->data[i] = scale * (((double)rand() * 2.0 / (double)RAND_MAX) - 1.0);
    }
    return m;
}

void free_matrix(matrix *m) {
    free(m->data);
    free(m);
}

matrix *copy_matrix(matrix *m) {
    double *data = malloc(m->cols * m->rows * sizeof(double));
    for (long i = 0; i < m->cols * m->rows; i++) {
        data[i] = m->data[i];
    }
    matrix *M = malloc(sizeof(matrix));
    M->cols = m->cols;
    M->rows = m->rows;
    M->data = data;
    return M;
}

matrix *make_matrix_from_array(double *array, long rows, long cols) {
    matrix *m = malloc(sizeof(matrix));
    m->cols = cols;
    m->rows = rows;
    m->data = array;
    return m;
}

void print_matrix(matrix *m, char *name) {
    matrix M = *m;
    printf("Name: %s Rows: %ld Cols: %ld\n", name, M.rows, M.cols);
    for (long r = 0; r < M.rows; r++) {
        for (long c = 0; c < M.cols; c++) {
            printf("%lf ", M.data[r * M.cols + c]);
        }
        printf("\n");
    }
    printf("\n");
}

matrix *scalar_multiply(matrix *m, double d) {
    matrix M = *m;
    matrix *result = create_value_matrix(M.rows, M.cols, 0.0);
    matrix R = *result;
    for (long i = 0; i < M.rows * M.cols; i++) {
        R.data[i] = M.data[i] * d;
    }
    return result;
}

matrix *add_matrices(matrix *m, matrix *n) {
    matrix M = *m;
    matrix N = *n;
    matrix *result = create_value_matrix(M.rows, M.cols, 0.0);
    matrix R = *result;
    for (long i = 0; i < M.rows * M.cols; i++) {
        R.data[i] = M.data[i] + N.data[i];
    }
    result->data = R.data;
    return result;
}

matrix *subtract_matrices(matrix *m, matrix *n) {
    matrix M = *m;
    matrix N = *n;
    matrix *result = create_value_matrix(M.rows, M.cols, 0.0);
    matrix R = *result;
    for (long i = 0; i < M.rows * M.cols; i++) {
        R.data[i] = M.data[i] - N.data[i];
    }
    result->data = R.data;
    return result;
}

matrix *hadamard_product(matrix *m, matrix *n) {
    matrix M = *m;
    matrix N = *n;
    matrix *result = create_value_matrix(M.rows, M.cols, 0.0);
    matrix R = *result;
    for (long i = 0; i < M.rows * M.cols; i++) {
        R.data[i] = M.data[i] * N.data[i];
    }
    result->data = R.data;
    return result;
}

matrix *multiply_matrices(matrix *m, matrix *n) {
    matrix M = *m;
    matrix N = *n;

    matrix *R = create_value_matrix(M.rows, N.cols, 0.0);
    matrix result = *R;
    for (long r = 0; r < result.rows; r++) {
        for (long c = 0; c < result.cols; c++) {
            double sum = 0.0;
            for (long i = 0; i < M.cols; i++) {  // Iterate over columns of M or rows of N
                sum += M.data[r * M.cols + i] * N.data[i * N.cols + c];
            }
            result.data[r * result.cols + c] = sum;  // Directly calculate the index here
        }
    }
    return R;
}


void scalar_multiply_inplace(matrix *m, double d) {
    matrix M = *m;
    for (long i = 0; i < M.rows * M.cols; i++) {
        M.data[i] *= d;
    }
}

void add_matrices_inplace(matrix *m, matrix *n) {
    matrix M = *m;
    matrix N = *n;
    for (long i = 0; i < M.rows * M.cols; i++) {
        M.data[i] += N.data[i];
    }
}

void subtract_matrices_inplace(matrix *m, matrix *n) {
    matrix M = *m;
    matrix N = *n;
    for (long i = 0; i < M.rows * M.cols; i++) {
        M.data[i] -= N.data[i];
    }
}

void hadamard_product_inplace(matrix *m, matrix *n) {
    matrix M = *m;
    matrix N = *n;
    for (long i = 0; i < M.rows * M.cols; i++) {
        M.data[i] *= N.data[i];
    }
}

void multiply_matrices_inplace(matrix *m, matrix *n, matrix *r) {
    matrix M = *m;
    matrix N = *n;

    matrix result = *r;
    for (long r = 0; r < result.rows; r++) {
        for (long c = 0; c < result.cols; c++) {
            double sum = 0.0;
            for (long i = 0; i < M.cols; i++) {  // Iterate over columns of M or rows of N
                sum += M.data[r * M.cols + i] * N.data[i * N.cols + c];
            }
            result.data[r * result.cols + c] = sum;  // Directly calculate the index here
        }
    }
}

matrix *transpose_matrix(matrix *m) {
    matrix M = *m;
    matrix *t = create_value_matrix(M.cols, M.rows, 0);
    matrix transposed = *t;

    for (long i = 0; i < M.rows; i++) {
        for (long j = 0; j < M.cols; j++) {
            transposed.data[j * transposed.cols + i] = m->data[i * M.cols + j];
        }
    }

    return t;
}

// int main() {
//     double id[] = {
//         2.0, 0.0, 0.0, 0.0,
//         0.0, 2.0, 0.0, 0.0,
//         0.0, 0.0, 2.0, 0.0,
//         0.0, 0.0, 0.0, 2.0
//     };
//     matrix *M = make_matrix_from_array(id, 4, 4);
//     matrix *v = create_value_matrix(1, 4, 1.0);
//     matrix *r = create_value_matrix(1, 4, 0.0);
//     multiply_matrices_inplace(v, M, r);
//     print_matrix(r, "Result");
// }