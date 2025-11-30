#include "fdlibm.h"

double pow(double x, double y) {
    return __ieee754_pow(x, y);
}

float powf(float x, float y) {
    // FIXME: Is this correct?
    return __ieee754_pow(x, y);
}
