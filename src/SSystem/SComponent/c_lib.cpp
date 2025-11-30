/**
 * c_lib.cpp
 *
 */

#include "SSystem/SComponent/c_lib.h"
#include "string.h"
#include "SSystem/SComponent/c_math.h"

/**
 * Copies a source block of memory to a destination block of memory
 * @param dst Pointer to destination memory
 * @param src Pointer to source data to be copied
 * @param num Number of bytes to copy
 */
void cLib_memCpy(void* dst, const void* src, unsigned long num) {
    memcpy(dst, src, num);
}

/**
 * Sets the first num bytes of given block of memory to specified value
 * @param ptr Pointer to block of memory
 * @param value Value to be set
 * @param num Number of bytes to set
 */
void cLib_memSet(void* ptr, int value, unsigned long num) {
    memset(ptr, value, num);
}

/**
 * Changes value by step towards target. Step amount is clamped between a min and max, and
 * scaled as a fraction of the remaining distance.
 * @param pvalue Pointer to value to change
 * @param target Target to move value towards
 * @param scale Fraction of remaining distance to scale step by
 * @param maxStep Maximum step amount
 * @param minStep Minimum step amount
 * @return The absolute value of remaining distance to target
 */
f32 cLib_addCalc(f32* pvalue, f32 target, f32 scale, f32 maxStep, f32 minStep) {
    f32 step = 0.0f;
    if (*pvalue != target) {
        step = scale * (target - *pvalue);
        if (step >= minStep || step <= -minStep) {
            if (step > maxStep) {
                step = maxStep;
            }
            if (step < -maxStep) {
                step = -maxStep;
            }
            *pvalue += step;
        } else {
            if (step > 0) {
                if (step < minStep) {
                    step = minStep;
                    *pvalue += step;
                    if (*pvalue > target) {
                        *pvalue = target;
                    }
                }
            } else {
                if (step > -minStep) {
                    step = -minStep;
                    *pvalue += step;
                    if (*pvalue < target) {
                        *pvalue = target;
                    }
                }
            }
        }
    }
    return fabsf(target - *pvalue);
}

/**
 * Changes value by step towards target. Step amount is clamped between a max and -max, and
 * scaled as a fraction of the remaining distance.
 * @param pvalue Pointer to value to change
 * @param target Target to move value towards
 * @param scale Fraction of remaining distance to scale step by
 * @param maxStep Maximum (+/-) step amount
 */
void cLib_addCalc2(f32* pvalue, f32 target, f32 scale, f32 maxStep) {
    if (*pvalue != target) {
        f32 step = scale * (target - *pvalue);
        if (step > maxStep) {
            step = maxStep;
        } else if (step < -maxStep) {
            step = -maxStep;
        }
        *pvalue += step;
    }
}

/**
 * Changes value by step towards zero. Step amount is clamped between a max and -max, and
 * scaled as a fraction of the remaining distance.
 * @param pvalue Pointer to value to change
 * @param scale Fraction of remaining distance to scale step by
 * @param maxStep Maximum (+/-) step amount
 */
void cLib_addCalc0(f32* pvalue, f32 scale, f32 maxStep) {
    f32 step = *pvalue * scale;
    if (step > maxStep) {
        step = maxStep;
    } else if (step < -maxStep) {
        step = -maxStep;
    }
    *pvalue -= step;
}

/**
 * Changes position by step towards target position. Step amount is clamped between a min and max,
 * and scaled as a fraction of the remaining distance.
 * @param ppos Pointer to position to change
 * @param target Target position to move towards
 * @param scale Fraction of remaining distance to scale step by
 * @param maxStep Maximum step amount
 * @param minStep Minimum step amount
 * @return The absolute value of remaining distance to target
 */
f32 cLib_addCalcPos(cXyz* ppos, const cXyz& target, f32 scale, f32 maxStep, f32 minStep) {
    if (*ppos != target) {
        cXyz diff = (*ppos - target);
        f32 step = diff.abs();
        if (step < minStep) {
            *ppos = target;
        } else {
            step *= scale;
            diff *= scale;
            if (!cLib_IsZero(step)) {
                if (step > maxStep) {
                    diff *= (maxStep / step);
                } else if (step < minStep) {
                    diff *= (minStep / step);
                }
                *ppos -= diff;
            } else {
                *ppos = target;
            }
        }
    }
    return ppos->abs(target);
}

/**
 * Changes position's X/Z components by step towards target position. Step amount is clamped
 * between a min and max, and scaled as a fraction of the remaining distance.
 * @param ppos Pointer to position to change
 * @param target Target position to move towards
 * @param scale Fraction of remaining distance to scale step by
 * @param maxStep Maximum step amount
 * @param minStep Minimum step amount
 * @return The absolute value of remaining distance to target
 */
f32 cLib_addCalcPosXZ(cXyz* ppos, const cXyz& target, f32 scale, f32 maxStep, f32 minStep) {
    if (ppos->x != target.x || ppos->z != target.z) {
        cXyz diff = (*ppos - target);
        f32 step = diff.absXZ();
        if (step < minStep) {
            ppos->x = target.x;
            ppos->z = target.z;
        } else {
            step *= scale;
            diff *= scale;
            if (!cLib_IsZero(step)) {
                if (step > maxStep) {
                    diff *= (maxStep / step);
                } else if (step < minStep) {
                    diff *= (minStep / step);
                }
                ppos->x -= diff.x;
                ppos->z -= diff.z;
            } else {
                ppos->x = target.x;
                ppos->z = target.z;
            }
        }
    }
    return (*ppos - target).absXZ();
}

/**
 * Changes position by step towards target position. Step amount is clamped between a max and -max
 * and scaled as a fraction of the remaining distance.
 * @param ppos Pointer to position to change
 * @param target Target position to move value towards
 * @param scale Fraction of remaining distance to scale step by
 * @param maxStep Maximum (+/-) step amount
 */
void cLib_addCalcPos2(cXyz* ppos, const cXyz& target, f32 scale, f32 maxStep) {
    if (*ppos != target) {
        cXyz diff = (*ppos - target) * scale;
        if (diff.abs() > maxStep) {
            diff = diff.normZP();
            diff *= maxStep;
        }
        *ppos -= diff;
    }
}

/**
 * Changes position's X/Z components by step towards target position. Step amount is clamped between
 * a max and -max and scaled as a fraction of the remaining distance.
 * @param ppos Pointer to position to change
 * @param target Target position to move value towards
 * @param scale Fraction of remaining distance to scale step by
 * @param maxStep Maximum (+/-) step amount
 */
void cLib_addCalcPosXZ2(cXyz* ppos, const cXyz& target, f32 scale, f32 maxStep) {
    if (ppos->x != target.x || ppos->z != target.z) {
        cXyz diff = (*ppos - target) * scale;
        f32 step = diff.absXZ();
        if (!cLib_IsZero(step)) {
            if (step > maxStep) {
                diff *= (maxStep / step);
            }
            ppos->x -= diff.x;
            ppos->z -= diff.z;
        }
    }
}

/**
 * Changes angle value by step towards target. Step amount is clamped between a min and max, and
 * scaled as a fraction of the remaining distance.
 * @param pvalue Pointer to angle value to change
 * @param target Target angle to move value towards
 * @param scale Fraction of remaining distance to scale step by
 * @param maxStep Maximum step amount
 * @param minStep Minimum step amount
 * @return The remaining distance to target
 */
s16 cLib_addCalcAngleS(s16* pvalue, s16 target, const s16 scale, s16 maxStep, s16 minStep) {
    s16 diff = target - *pvalue;
    if (*pvalue != target) {
        s16 step = (diff) / scale;
        if (step > minStep || step < -minStep) {
            if (step > maxStep) {
                step = maxStep;
            }
            if (step < -maxStep) {
                step = -maxStep;
            }
            *pvalue += step;
        } else {
            if (0 <= diff) {
                *pvalue += minStep;
                diff = target - *pvalue;
                if (0 >= diff) {
                    *pvalue = target;
                }
            } else {
                *pvalue -= minStep;
                diff = target - *pvalue;
                if (0 <= diff) {
                    *pvalue = target;
                }
            }
        }
    }
    return target - *pvalue;
}

/**
 * Changes angle value by step towards target angle. Step amount is clamped between a max and -max
 * and scaled as a fraction of the remaining distance.
 * @param pvalue Pointer to angle value to change
 * @param target Target angle to move value towards
 * @param scale Fraction of remaining distance to scale step by
 * @param maxStep Maximum (+/-) step amount
 */
void cLib_addCalcAngleS2(s16* pvalue, s16 target, s16 scale, s16 maxStep) {
    s16 diff = target - *pvalue;
    s16 step = diff / scale;
    if (step > maxStep) {
        *pvalue += maxStep;
    } else if (step < -maxStep) {
        *pvalue -= maxStep;
    } else {
        *pvalue += step;
    }
}

/**
 * Changes unsigned char value by step towards target
 * @param pvalue Pointer to value to change
 * @param target Target to move value towards
 * @param step Step amount
 * @return TRUE when target is reached, FALSE otherwise
 */
int cLib_chaseUC(u8* pvalue, u8 target, u8 step) {
    if (step) {
        s16 wideValue = *pvalue;
        s16 wideTarget = target;
        s16 wideStep;
        if (wideValue > wideTarget) {
            wideStep = -step;
        } else {
            wideStep = step;
        }
        wideValue += wideStep;
        if (wideStep * (wideValue - wideTarget) >= 0) {
            *pvalue = target;
            return 1;
        } else {
            *pvalue = wideValue;
        }
    } else if (*pvalue == target) {
        return 1;
    }
    return 0;
}

/**
 * Changes signed short value by step towards target
 * @param pvalue Pointer to value to change
 * @param target Target to move value towards
 * @param step Step amount
 * @return TRUE when target is reached, FALSE otherwise
 */
int cLib_chaseS(s16* pvalue, s16 target, s16 step) {
    if (step) {
        if (*pvalue > target) {
            step = -step;
        }
        *pvalue += step;
        if (step * (*pvalue - target) >= 0) {
            *pvalue = target;
            return 1;
        }
    } else if (*pvalue == target) {
        return 1;
    }
    return 0;
}

/**
 * Changes float value by step towards target
 * @param pvalue Pointer to value to change
 * @param target Target to move value towards
 * @param step Step amount
 * @return TRUE when target is reached, FALSE otherwise
 */
int cLib_chaseF(f32* pvalue, f32 target, f32 step) {
    if (step) {
        if (*pvalue > target) {
            step = -step;
        }
        *pvalue += step;
        if (step * (*pvalue - target) >= 0) {
            *pvalue = target;
            return 1;
        }
    } else if (*pvalue == target) {
        return 1;
    }
    return 0;
}

/**
 * Changes position by step towards target
 * @param pvalue Pointer to position to change
 * @param target Target position to move towards
 * @param step Step amount
 * @return TRUE when target is reached, FALSE otherwise
 */
int cLib_chasePos(cXyz* pvalue, const cXyz& target, f32 step) {
    if (step) {
        cXyz diff = *pvalue - target;
        f32 diffF = diff.abs();
        if (cLib_IsZero(diffF) || diffF <= step) {
            *pvalue = target;
            return 1;
        }
        *pvalue -= diff * (step / diffF);
    } else if (*pvalue == target) {
        return 1;
    }
    return 0;
}

/**
 * Changes position's X/Z components by step towards target
 * @param pvalue Pointer to position to change
 * @param target Target position to move towards
 * @param step Step amount
 * @return TRUE when target is reached, FALSE otherwise
 */
int cLib_chasePosXZ(cXyz* pvalue, const cXyz& target, f32 step) {
    cXyz diff = *pvalue - target;
    diff.y = 0;
    f32 diffF = diff.absXZ();
    if (step) {
        if (cLib_IsZero(diffF) || diffF <= step) {
            *pvalue = target;
            return 1;
        }
        *pvalue -= diff * (step / diffF);
    } else if (cLib_IsZero(diffF)) {
        return 1;
    }
    return 0;
}

/**
 * Changes angle value by step towards target
 * @param pvalue Pointer to value to change
 * @param target Target to move value towards
 * @param step Step amount
 * @return TRUE when target is reached, FALSE otherwise
 */
int cLib_chaseAngleS(s16* pvalue, s16 target, s16 step) {
    if (step) {
        if ((s16)(*pvalue - target) > 0) {
            step = -step;
        }
        *pvalue += step;
        if (step * (s16)(*pvalue - target) >= 0) {
            *pvalue = target;
            return 1;
        }
    } else if (*pvalue == target) {
        return 1;
    }
    return 0;
}

/**
 * Gets the target y-angle from position A to position B
 * @param lhs Pointer to position A
 * @param rhs Pointer to position B
 * @return Y-angle from position A to position B
 */
s16 cLib_targetAngleY(const Vec* lhs, const Vec* rhs) {
    return cM_atan2s(rhs->x - lhs->x, rhs->z - lhs->z);
}

/**
 * Gets the target y-angle from position A to position B
 * @param lhs Reference to position A
 * @param rhs Reference to position B
 * @return Y-angle from position A to position B
 */
s16 cLib_targetAngleY(const Vec& lhs, const Vec& rhs) {
    return cM_atan2s(rhs.x - lhs.x, rhs.z - lhs.z);
}

/**
 * Gets the target x-angle from position A to position B
 * @param lhs Pointer to position A
 * @param rhs Pointer to position B
 * @return X-Angle from position A to position B
 */
s16 cLib_targetAngleX(cXyz const* lhs, cXyz const* rhs) {
    cXyz diff = *rhs - *lhs;
    f32 f1 = sqrtf(diff.getMagXZ());
    return cM_atan2s(diff.y, f1);
}

/**
 * Adds an offset to a source position in a given angle direction and places the result in pdest
 * @param pdest The resulting position
 * @param psrc The source position
 * @param angle The direction to offset psrc in
 * @param vec The offset cXyz to add to psrc
 */
void cLib_offsetPos(cXyz* pdest, cXyz const* psrc, s16 angle, cXyz const* vec) {
    f32 cos = cM_scos(angle);
    f32 sin = cM_ssin(angle);
    pdest->x = psrc->x + (vec->x * cos + vec->z * sin);
    pdest->y = psrc->y + vec->y;
    pdest->z = psrc->z + (vec->z * cos - vec->x * sin);
}

/**
 * Gets the target x-angle from position A to position B
 * @param lhs Pointer to position A
 * @param rhs Pointer to position B
 * @return X-Angle from position A to position B
 */
s32 cLib_distanceAngleS(s16 x, s16 y) {
    return abs(static_cast<s16>(x - y));
}

static Mtx mtx[10];

Mtx* calc_mtx = mtx;

/**
 * Initializes calc_mtx to mtx stack
 */
void MtxInit() {
    calc_mtx = mtx;
}

void MtxTrans(f32 x_trans, f32 y_trans, f32 z_trans, u8 param_3) {
    if (param_3 == 0) {
        MTXTrans(*calc_mtx, x_trans, y_trans, z_trans);
    } else {
        Mtx mtx;
        MTXTrans(mtx, x_trans, y_trans, z_trans);
        MTXConcat(*calc_mtx, mtx, *calc_mtx);
    }
}

void MtxScale(f32 x_trans, f32 y_trans, f32 z_trans, u8 param_3) {
    if (param_3 == 0) {
        MTXScale(*calc_mtx, x_trans, y_trans, z_trans);
    } else {
        Mtx mtx;
        MTXScale(mtx, x_trans, y_trans, z_trans);
        MTXConcat(*calc_mtx, mtx, *calc_mtx);
    }
}

/**
 * Multiplies a src position by the calc_mtx and puts the result in dest
 * @param src The src position to be multiplied
 * @param dest The resulting multiplied position
 */
void MtxPosition(cXyz* src, cXyz* dest) {
    MTXMultVec(*calc_mtx, src, dest);
}

void MtxPush() {
    Mtx mtx;
    MTXCopy(*calc_mtx, mtx);
    calc_mtx++;
    MTXCopy(mtx, *calc_mtx);
}

Mtx* MtxPull() {
    return calc_mtx--;
}
