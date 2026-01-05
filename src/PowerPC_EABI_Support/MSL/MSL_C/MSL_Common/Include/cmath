#ifndef MSL_CMATH_H_
#define MSL_CMATH_H_

#include <float.h>

#define NAN (*(float*) __float_nan)
#define HUGE_VALF (*(float*) __float_huge)

#if !PLATFORM_GCN
#define HUGE_VAL (*(double*)__double_huge)
#endif

#define M_PI 3.14159265358979323846f
#define M_SQRT3 1.73205f

#define DEG_TO_RAD(degrees) (degrees * (M_PI / 180.0f))
#define RAD_TO_DEG(radians) (radians * (180.0f / M_PI))

#ifdef __cplusplus
extern "C" {
#endif

extern double __frsqrte(double);
extern float __fres(float);
extern double __fabs(double);
extern float __fabsf(float);

int abs(int);
double acos(double);
float acosf(float);
double asin(double);
double atan(double);
double atan2(double, double);
double ceil(double);
double copysign(double, double);
double cos(double);
float cosf(float);
double exp(double);
double floor(double);
double fmod(double, double);
double frexp(double, int*);
double ldexp(double, int);
double modf(double, double*);
double pow(double, double);
float powf(float, float);
double sin(double);
float sinf(float);
double sqrt(double);
double tan(double);
float tanf(float);
double log10(double);

inline double fabs(double f) {
    return __fabs(f);
}

inline long double fabsl(long double x) {
    return fabs((double)x);
}



MSL_INLINE float acosf(float x) {
    return acos(x);
}

MSL_INLINE float atan2f(float y, float x) {
    return (float)atan2(y, x);
}

MSL_INLINE float ceilf(float num) {
    return ceil(num);
}

MSL_INLINE float cosf(float x) {
    return cos(x);
}

MSL_INLINE float expf(float x) {
    return exp(x);
}

MSL_INLINE float floorf(float num) {
    return floor(num);
}

MSL_INLINE float powf(float x, float y) {
    return pow(x, y);
}

MSL_INLINE float sinf(float x) {
    return sin(x);
}

MSL_INLINE float fabsf(float f) {
    return (float)fabs((double)f);
}

MSL_INLINE float fmodf(float f1, float f2) {
    return fmod(f1, f2);
}

MSL_INLINE float log10f(float x) {
    return log10(x);
}

#include "global.h"
#if PLATFORM_WII || PLATFORM_SHIELD
MSL_INLINE float sqrtf(float mag) {
    return sqrt(mag);
}
#else
#ifdef DECOMPCTX
// Hack to mitigate fake mismatches when building from decompctx output
// (which doesn't support precompiled headers).
//
// When built without a PCH, these constants would end up .rodata instead of .data
// which causes a variety of knock-on effects in individual functions' assembly.
//
// Making them into globals is a bit of a hack, but it generally fixes later
// .data and .rodata offsets and allows individual functions to match.
static double _half = 0.5;
static double _three = 3.0;
#endif
MSL_INLINE float sqrtf(float mag) {
#ifndef DECOMPCTX
    // part of the same hack, these are defined outside of the function when using decompctx
    static const double _half = 0.5;
    static const double _three = 3.0;
#endif
    if (mag > 0.0f) {
        double dmag = (double)mag;
        double tmpd = __frsqrte(dmag);
        tmpd = _half * tmpd * (_three - tmpd * tmpd * dmag);
        tmpd = _half * tmpd * (_three - tmpd * tmpd * dmag);
        tmpd = _half * tmpd * (_three - tmpd * tmpd * dmag);
        return (float)(dmag * tmpd);
    } else if (mag < 0.0) {
        return NAN;
    } else if (isnan(mag)) {
        return NAN;
    } else {
        return mag;
    }
}
#endif

MSL_INLINE float tanf(float x) {
    return tan(x);
}

#ifdef __cplusplus
};
#endif

#endif
