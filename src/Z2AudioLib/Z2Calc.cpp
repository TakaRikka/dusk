#include "Z2AudioLib/Z2Calc.h"
#include "JSystem/JMath/random.h"
#include <math.h>

f32 Z2Calc::linearTransform(f32 inValue, f32 inMin, f32 inMax, f32 outMin, f32 outMax, bool noClamp) {
    f32 transformed = outMin + ((inValue - inMin) * ((outMax - outMin) / (inMax - inMin)));
    if (noClamp) {
        return transformed;
    }

    if (outMin < outMax) {
        return transformed > outMax ? outMax :
               transformed < outMin ? outMin :
                                      transformed;
    } else {
        return transformed > outMin ? outMin :
               transformed < outMax ? outMax :
                                      transformed;
    }
}

const f32 Z2Calc::cEqualCSlope = 1.0f;

const f32 Z2Calc::cEqualPSlope = 0.5f;

f32 Z2Calc::getParamByExp(f32 value, f32 inMin, f32 inMax, f32 exponent, f32 outMin, f32 outMax, Z2Calc::CurveSign curveSign) {
    if (curveSign == Z2Calc::CURVE_POSITIVE) {
        value = linearTransform(value, inMin, inMax, 0.0f, exponent, true);
        value = expf(value);
        value = linearTransform(value, 1.0f, expf(exponent), outMin, outMax, true);
    } else if (curveSign == Z2Calc::CURVE_NEGATIVE) {
        value = linearTransform(value, inMin, inMax, exponent, 0.0f, true);
        value = expf(value);
        value = linearTransform(value, expf(exponent), 1.0f, outMin, outMax, true);
    } else {
        value = linearTransform(value, inMin, inMax, outMin, outMax, false);
    }

    return value > outMax ? outMax :
           value < outMin ? outMin :
                            value;
}

f32 Z2Calc::getRandom(f32 magnitude, f32 exponent, f32 bias) {
    f32 posBias = 2.0f * bias;
    f32 negBias = (1.0f - bias) * -2.0f;
    magnitude *= Z2Calc::getRandom_0_1() < bias ? posBias : negBias;

    f32 exprnd = pow(Z2Calc::getRandom_0_1(), exponent);
    return exprnd * magnitude;
}

f32 Z2Calc::getRandom_0_1() {
    static JMath::TRandom_<JMath::TRandom_fast_> oRandom(0);
    return oRandom.get_ufloat_1();
}

void Z2Calc::FNoise1f::setParam(f32 value, f32 threshold, f32 step) {
    value_ = value;
    targetValue_ = value;
    threshold_ = threshold;
    step_ = step;
}

f32 Z2Calc::FNoise1f::tau(f32 input) {
    f32 var_f30 = 1.0f;

    f32 result = input + (input * input);
    if (result > var_f30) {
        result = result - var_f30;
        if (input < threshold_) {
            result += threshold_;
        }
        return result;
    }

    if (input < threshold_) {
        result += threshold_;
    }

    return result;
}

f32 Z2Calc::FNoise1f::calcNoise1f() {
    if (value_ < targetValue_) {
        value_ += step_;
        if (value_ < targetValue_) {
            return value_;
        }
    } else {
        value_ -= step_;
        if (value_ > targetValue_) {
            return value_;
        }
    }

    value_ = targetValue_;
    targetValue_ = tau(value_);
    return value_;
}
