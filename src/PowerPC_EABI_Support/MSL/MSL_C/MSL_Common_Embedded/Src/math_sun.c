#include <cmath>

double scalbn(double x, int n) {
    double mant;
    int exp;
    double result;
     
    mant = frexp(x, &exp);
    exp += n;
    result = ldexp(mant, exp);
    
    return result;
}
