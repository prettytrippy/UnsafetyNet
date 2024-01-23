#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "matrix.h"
#include "activation_functions.h"
#include "loss_functions.h"
#include "schedulers.h"

matrix empty_matrix = {
    .cols = 0,
    .rows = 0,
    .data = NULL,
};

typedef struct {
    matrix weights;
    matrix_function activation;
    matrix_function activation_derivative;
    long output_size;
    matrix data;
} layer;

void free_layer(layer l) {
    free(l.weights.data);
    free(l.data.data);
    return;
}

typedef struct {
    long num_layers;
    layer *layers;
} neural_network;

neural_network *create_neural_network(long *layer_sizes, long num_layers, long input_rows, char **activations) {

    layer *layers = malloc(sizeof(layer) * num_layers);

    for (long i = 0; i < num_layers; i++) {
        matrix *weights = &empty_matrix;
        if (i > 0) {
            weights = create_He_initialized_matrix(layer_sizes[i-1], layer_sizes[i]);
        }

        matrix *data = create_random_matrix(input_rows, layer_sizes[i]);
        layer current_layer = {
            .weights = *weights,
            .activation = get_activation(activations[i]),
            .activation_derivative = get_activation_derivative(activations[i]),
            .output_size = layer_sizes[i],
            .data = *data,
        };
        if (i > 0) free(weights);
        free(data);
        layers[i] = current_layer;
    }

    neural_network *net = malloc(sizeof(neural_network));
    net->layers = layers;
    net->num_layers = num_layers;
    return net;
}

void free_neural_network(neural_network *n) {
    neural_network N = *n;
    for (long i = 0; i < N.num_layers; i++) {
        free_layer(N.layers[i]);
    }
    free(n);
}

matrix forward_pass(matrix *input_pointer, neural_network *n) {
    neural_network N = *n;
    N.layers[0].data = *input_pointer;
    for (long i = 1; i < N.num_layers; i++) {
        layer current_layer = N.layers[i];
        layer previous_layer = N.layers[i-1];
        multiply_matrices_inplace(&previous_layer.data, &current_layer.weights, &current_layer.data);
        current_layer.data = current_layer.activation(current_layer.data);
    }
    return N.layers[N.num_layers - 1].data;
}

void backward_pass(matrix *error, neural_network *n, double learning_rate) {
    neural_network N = *n;

    for (long i = N.num_layers - 1; i > 0; i--) {
        layer current_layer = N.layers[i];
        layer previous_layer = N.layers[i-1];

        matrix derivative = current_layer.activation_derivative(current_layer.data);
        hadamard_product_inplace(&derivative, error);
        matrix *weight_gradient = multiply_matrices(transpose_matrix(&previous_layer.data), &derivative);
        scalar_multiply_inplace(weight_gradient, learning_rate);
        subtract_matrices_inplace(&current_layer.weights, weight_gradient);
        free(error->data);
        free_matrix(weight_gradient);
        error = multiply_matrices(&derivative, transpose_matrix(&current_layer.weights));
    }
}

void train_network(matrix *input, matrix *expected_output, neural_network *n, loss_function loss_derivative, double learning_rate) {
    matrix observed_output = forward_pass(input, n);
    matrix error = loss_derivative(n->layers[n->num_layers-1].data, *expected_output);
    // print_matrix(&observed_output, "Observed");
    // print_matrix(expected_output, "Expected");
    // printf("Error: %lf\n", mean_squared_error(*expected_output, observed_output));
    backward_pass(&error, n, learning_rate);
}

int main() {
    long sizes[] = {256, 256};
    char *activations[] = {"ReLU", "Linear"};
    neural_network *N = create_neural_network(sizes, 2, 1, activations);

    for (long i = 0; i < limit; i++) {
        train_network(inputs[i], expecteds[i], N, loss_derivative, lr_scheduler(i, 0.0001));
    }
}

// gcc -o net loss_functions.c neural_net.c activation_functions.c matrix.c -lm
