#include <math.h>
#include "schedulers.h"

double max_lr = 0.001;

double constant(long i, double scale) {
    return scale;
}

double rational(long i, double scale) {
    return scale/(i + scale/max_lr);
}

double weird_polyfit_thingy(long i) {
    double d = i * i;
    return 0.01 * (100.0 * d + i)/(100.0 * d * i + d + 400000.0);
}
