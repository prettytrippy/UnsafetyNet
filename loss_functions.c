#include <stdlib.h>
#include <stdio.h>
#include "matrix.h"
#include <math.h>
#include <string.h>
#include "loss_functions.h"


double mean_squared_error(matrix expected, matrix observed) {
    double mse = 0.0;
    int size = expected.rows * expected.cols;
    for (int i = 0; i < size; i++) {
        double diff = expected.data[i] - observed.data[i];
        mse += diff * diff;
    }
    mse /= size;
    return mse;
}

matrix mean_squared_error_derivative(matrix expected, matrix observed) {
    int size = expected.rows * expected.cols;
    matrix derivative = {
        .rows = expected.rows,
        .cols = expected.cols,
        .data = (double*)malloc(size * sizeof(double))
    };

    for (int i = 0; i < size; i++) {
        derivative.data[i] = 2.0 * (observed.data[i] - expected.data[i]) / size;
    }

    return derivative;
}

