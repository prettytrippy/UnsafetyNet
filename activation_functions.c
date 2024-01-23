#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include <math.h>
#include "activation_functions.h"
#include <string.h>

matrix linear(matrix m) {
    double *arr = malloc(sizeof(double) * m.rows * m.cols);
    for (long i = 0; i < m.rows * m.cols; i++) arr[i] = m.data[i];
    matrix ret = {
        .rows = m.rows,
        .cols = m.cols,
        .data = arr
    };
    return ret;
}

matrix linear_derivative(matrix m) {
    double *arr = malloc(sizeof(double) * m.rows * m.cols);
    for (long i = 0; i < m.rows * m.cols; i++) arr[i] = 1.0;
    matrix ret = {
        .rows = m.rows,
        .cols = m.cols,
        .data = arr
    };
    return ret;
}

void softmax_helper(double *arr, long start, long end) {
    double max_val = arr[start];
    for (long i = start + 1; i < end; i++) {
        if (arr[i] > max_val) {
            max_val = arr[i];
        }
    }

    double sum = 0.0;
    for (long i = start; i < end; i++) {
        arr[i] = exp(arr[i] - max_val);  // Subtract max_val for numerical stability
        sum += arr[i];
    }
    for (long i = start; i < end; i++) {
        arr[i] /= sum;
    }
}

matrix softmax(matrix m) {
    // the helper can be combined with this to work faster and perform fewer passes
    double *arr = malloc(sizeof(double) * m.rows * m.cols);
    for (long i = 0; i < m.rows * m.cols; i++) arr[i] = m.data[i];
    for (long r = 0; r < m.rows; r++) {
        softmax_helper(arr, r * m.cols, (r + 1) * m.cols);
    }
    matrix ret = {
        .rows = m.rows,
        .cols = m.cols,
        .data = arr
    };
    return ret;
}

matrix softmax_derivative(matrix input) {
    matrix softmax_output = softmax(input);
    matrix result = {
        .rows = input.rows,
        .cols = input.cols,
        .data = (double*)malloc(input.rows * input.cols * sizeof(double))
    };

    for (int i = 0; i < input.rows; i++) {
        for (int j = 0; j < input.cols; j++) {
            for (int k = 0; k < input.cols; k++) {
                if (j == k) {
                    // Diagonal elements: s_i * (1 - s_i)
                    double softmax_val = softmax_output.data[i * input.cols + j];
                    result.data[i * input.cols + j] += softmax_val * (1.0 - softmax_val);
                } else {
                    // Off-diagonal elements: -s_i * s_j
                    double softmax_i = softmax_output.data[i * input.cols + j];
                    double softmax_j = softmax_output.data[i * input.cols + k];
                    result.data[i * input.cols + j] -= softmax_i * softmax_j;
                }
            }
        }
    }
    free(softmax_output.data);
    return result;
}


matrix relu(matrix m) {
    double *arr = malloc(sizeof(double) * m.rows * m.cols);
    for (long i = 0; i < m.rows * m.cols; i++) arr[i] = m.data[i] * (m.data[i] > 0.0);
    matrix ret = {
        .rows = m.rows,
        .cols = m.cols,
        .data = arr
    };
    return ret;
}

matrix relu_derivative(matrix m) {
    double *arr = malloc(sizeof(double) * m.rows * m.cols);
    for (long i = 0; i < m.rows * m.cols; i++) arr[i] = (m.data[i] > 0.0);
    matrix ret = {
        .rows = m.rows,
        .cols = m.cols,
        .data = arr
    };
    return ret;
}


char *activation_function_names[] = {"ReLU", "Softmax", "Linear", "STOP"};
matrix_function activations[] = {relu, softmax, linear};
matrix_function activation_derivatives[] = {relu_derivative, softmax_derivative, linear_derivative};


long get_activation_idx(char *name) {
    long idx = 0;
    while (strcmp(name, activation_function_names[idx])) {
        if (!strcmp(activation_function_names[idx], "STOP")) {
            fprintf(stderr, "No such activation function found.\n");
            return -1;
        }
        idx++;
    }
    return idx;
}

matrix_function get_activation(char *name) {
    long idx = get_activation_idx(name);
    return activations[idx];
}

matrix_function get_activation_derivative(char *name) {
    long idx = get_activation_idx(name);
    return activation_derivatives[idx];
}
