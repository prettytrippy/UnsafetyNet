#ifndef LOSS_FUNCTIONS_H
#define LOSS_FUNCTIONS_H

#include "matrix.h"

typedef matrix (*loss_function)(matrix, matrix);

double mean_squared_error(matrix e, matrix o);
matrix mean_squared_error_derivative(matrix e, matrix o);

#endif 
