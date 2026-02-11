#ifndef Z2CALC_H
#define Z2CALC_H

#include <dolphin/types.h>
#include "m_Do/m_Do_lib.h"

namespace Z2Calc {
#ifdef DECOMPCTX
// Hack to mitigate fake mismatches when building from decompctx output -
// see comment in sqrtf in math.h
static Vec cNullVec = {0.0f, 0.0f, 0.0f};
#else
static const Vec cNullVec = {0.0f, 0.0f, 0.0f};
#endif

enum CurveSign {
    CURVE_NEGATIVE = 0,
    CURVE_POSITIVE = 1,
    CURVE_LINEAR   = 2,
};

struct FNoise1f {
    FNoise1f(f32 value = 0.1f, f32 threshold = 0.0f, f32 step = 0.01f) {
        value_ = value;
        targetValue_ = value;
        step_ = step;
        threshold_ = threshold;
    }

    void setParam(f32 value, f32 threshold, f32 step);
    f32 tau(f32 input);
    f32 calcNoise1f();

    f32 value_;
    f32 targetValue_;
    f32 step_;
    f32 threshold_;
};

f32 linearTransform(f32 inValue, f32 inMin, f32 inMax, f32 outMin, f32 outMax, bool noClamp);
f32 getParamByExp(f32 value, f32 inMin, f32 inMax, f32 exponent, f32 outMin, f32 outMax, CurveSign curveSign);
f32 getRandom(f32 magnitude, f32 exponent, f32 bias);
f32 getRandom_0_1();

extern const f32 cEqualCSlope;
extern const f32 cEqualPSlope;
}  // namespace Z2Calc

#endif /* Z2CALC_H */
