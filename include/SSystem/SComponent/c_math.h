#ifndef C_MATH_H
#define C_MATH_H

#include "JSystem/JMath/JMATrigonometric.h"

#ifndef __MWERKS__
#include <limits>
#ifndef FLT_MAX
#define FLT_MAX std::numeric_limits<float>::max()
#endif
#endif

s16 cM_rad2s(float rad);
u16 U_GetAtanTable(float, float);
s16 cM_atan2s(float y, float x);
float cM_atan2f(float y, float x);
void cM_initRnd(int, int, int);
float cM_rnd();
float cM_rndF(float);
float cM_rndFX(float);
void cM_initRnd2(int, int, int);
float cM_rnd2();
float cM_rndF2(float);
float cM_rndFX2(float);

inline f32 cM_scos(s16 x) {
    return JMASCos(x);
}

inline f32 cM_ssin(s16 x) {
    return JMASSin(x);
}

inline s16 cM_deg2s(f32 deg) {
    return deg * 182.04445f;
}

inline s16 i_cM_deg2s(f32 deg) {
    return deg;
}

inline f32 cM_fcos(f32 x) {
    return JMASCos(cM_rad2s(x));
}

inline f32 cM_fsin(f32 x) {
    return JMASSin(cM_rad2s(x));
}

inline f32 cM_sht2d(f32 v) {
    return v * 0.005493164f;
}

inline f32 cM_s2rad(s16 x) {
    return x * 9.58738e-05f;
}

#endif /* C_MATH_H */
