#ifndef MATRIX_H
#define MATRIX_H

typedef struct {
    long rows;
    long cols;
    double *data;
} matrix;

matrix *create_value_matrix(long rows, long cols, double value);
matrix *create_random_matrix(long rows, long cols);
matrix *create_scaled_random_matrix(long rows, long cols, double scale);
matrix *create_He_initialized_matrix(long rows, long cols);

void free_matrix(matrix *m);
void print_matrix(matrix *m, char *name);
matrix *copy_matrix(matrix *m);
matrix *make_matrix_from_array(double *array, long rows, long cols);

matrix *scalar_multiply(matrix *m, double d);
matrix *add_matrices(matrix *m, matrix *n);
matrix *subtract_matrices(matrix *m, matrix *n);
matrix *hadamard_product(matrix *m, matrix *n);
matrix *multiply_matrices(matrix *m, matrix *n);

void scalar_multiply_inplace(matrix *m, double d);
void add_matrices_inplace(matrix *m, matrix *n);
void subtract_matrices_inplace(matrix *m, matrix *n);
void hadamard_product_inplace(matrix *m, matrix *n);
void multiply_matrices_inplace(matrix *m, matrix *n, matrix *r);

matrix *transpose_matrix(matrix *m);

#endif /* MATRIX_H */
