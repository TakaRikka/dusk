#include "math_ppc.h"
#include "fdlibm.h"

// TODO: where should this go?
#ifndef DECL_WEAK
#if defined(__MWERKS__)
#define DECL_WEAK __declspec(weak)
#elif defined(__GNUC__)
#define DECL_WEAK __attribute__((weak))
#elif defined(_MSC_VER)
#define DECL_WEAK
#else
#error unknown compiler
#endif
#endif

double nan(const char* arg) {}

DECL_WEAK float acosf(float x) {
    return acos(x);
}

DECL_WEAK float cosf(float x) {
    return cos(x);
}

DECL_WEAK float sinf(float x) {
    return sin(x);
}

DECL_WEAK float tanf(float x) {
    return tan(x);
}
