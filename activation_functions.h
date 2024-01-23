#ifndef ACTIVATION_FUNCTIONS_H
#define ACTIVATION_FUNCTIONS_H

#include "matrix.h"

typedef double (*scalar_function)(double);
typedef matrix (*matrix_function)(matrix);

matrix_function get_activation(char *name);
matrix_function get_activation_derivative(char *name);

#endif 
