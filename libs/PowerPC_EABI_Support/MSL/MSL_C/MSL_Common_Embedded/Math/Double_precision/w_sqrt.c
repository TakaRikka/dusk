#include "fdlibm.h"

double sqrt(double x) {
    return __ieee754_sqrt(x);
}
