#include "fdlibm.h"

double exp(double x) {
    return __ieee754_exp(x);
}
