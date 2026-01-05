#ifndef _MSL_COMMON_FLOAT_H
#define _MSL_COMMON_FLOAT_H

#include <fdlibm.h>
#include "global.h"

#define FP_SNAN 0
#define FP_QNAN 1
#define FP_INFINITE 2
#define FP_ZERO 3
#define FP_NORMAL 4
#define FP_SUBNORMAL 5

#define FP_NAN FP_QNAN

#if __REVOLUTION_SDK__
#define fpclassify(x) \
	((sizeof(x) == sizeof(float)) ? __fpclassifyf((float)(x)) : \
	(sizeof(x) == sizeof(double)) ? __fpclassifyd((double)(x)) : \
	__fpclassifyl((long double)(x)) )
#else
#define fpclassify(x) \
	((sizeof(x) == sizeof(float)) ? __fpclassifyf((float)(x)) : \
	__fpclassifyd((double)(x)) )
#endif

#define signbit(x) ((sizeof(x) == sizeof(float)) ? __signbitf(x) : __signbitd(x))
#define isfinite(x) ((fpclassify(x) > FP_INFINITE))
#define isnan(x) (fpclassify(x) == FP_NAN)
#define isinf(x) (fpclassify(x) == FP_INFINITE)

#define __signbitf(x) ((int)(__HI(x) & 0x80000000))

// TODO: OK?
#define __signbitd(x) ((int)(__HI(x) & 0x80000000))

extern int __float_nan[];
extern int __float_huge[];
extern int __float_max[];
extern int __float_epsilon[];

#if !PLATFORM_GCN
extern int __double_huge[];
#endif

#ifdef __cplusplus
extern "C" {
#endif

inline int __fpclassifyf(float __value) {
    switch (*(int*)&__value & 0x7f800000) {
    case 0x7f800000:
        if ((*(int*)&__value & 0x7fffff) != 0) {
            return FP_QNAN;
        }
        return FP_INFINITE;

    case 0:
        if ((*(int*)&__value & 0x7fffff) != 0) {
            return FP_SUBNORMAL;
        }
        return FP_ZERO;
    }

    return FP_NORMAL;
}

inline int __fpclassifyd(double __value) {
    switch (__HI(__value) & 0x7ff00000) {
	case 0x7ff00000: {
		if ((__HI(__value) & 0x000fffff) || (__LO(__value) & 0xffffffff))
			return FP_QNAN;
		else
			return FP_INFINITE;
		break;
	}
	case 0: {
		if ((__HI(__value) & 0x000fffff) || (__LO(__value) & 0xffffffff))
			return FP_SUBNORMAL;
		else
			return FP_ZERO;
		break;
	}
	}
	return FP_NORMAL;
}

// Stripped function.
int __fpclassifyl(long double __value);

#ifdef __cplusplus
}; // extern "C"
#endif

#define FLT_MANT_DIG   24
#define FLT_DIG        6
#define FLT_MIN_EXP    (-125)
#define FLT_MIN_10_EXP (-37)
#define FLT_MAX_EXP    128
#define FLT_MAX_10_EXP 38

#if DEBUG
#define FLT_MAX 3.4028235e38f
#define FLT_EPSILON 1.1920929e-7f
#else
#define FLT_MAX (*(float*) __float_max)
#define FLT_EPSILON (*(float*) __float_epsilon)
#endif

#define DBL_MANT_DIG   53
#define DBL_DIG        15
#define DBL_MIN_EXP    (-1021)
#define DBL_MIN_10_EXP (-308)
#define DBL_MAX_EXP    1024
#define DBL_MAX_10_EXP 308

#define LDBL_MANT_DIG 53
#define LDBL_DIG 15
#define LDBL_MIN_EXP (-1021)
#define LDBL_MIN_10_EXP (-308)
#define LDBL_MAX_EXP 1024
#define LDBL_MAX_10_EXP 308

#define LDBL_MAX 0x1.fffffffffffffP1023L
#define LDBL_EPSILON 0x1.0000000000000P-52L
#define LDBL_MIN 0x1.0000000000000P-1022L

#endif /* _MSL_COMMON_FLOAT_H */
