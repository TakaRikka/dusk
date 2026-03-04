#include "fdlibm.h"

double asin(double x) {
    return __ieee754_asin(x);
}
