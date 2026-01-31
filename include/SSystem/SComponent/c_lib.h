#ifndef C_LIB_H_
#define C_LIB_H_

#include "SSystem/SComponent/c_xyz.h"
#include <dolphin/mtx.h>
#include "SSystem/SComponent/c_math.h"

inline bool cLib_IsZero(f32 value) {
    return fabsf(value) < 8e-11f;
}

void cLib_memCpy(void* dst, const void* src, u32 size);
void cLib_memSet(void* ptr, int value, u32 size);

f32 cLib_addCalc(f32* o_value, f32 target, f32 scale, f32 maxStep, f32 minStep);
void cLib_addCalc2(f32* o_value, f32 target, f32 scale, f32 maxStep);
void cLib_addCalc0(f32* o_value, f32 scale, f32 maxStep);
f32 cLib_addCalcPos(cXyz* o_value, cXyz const& target, f32 scale, f32 maxStep, f32 minStep);
f32 cLib_addCalcPosXZ(cXyz* o_value, cXyz const& target, f32 scale, f32 maxStep, f32 minStep);
void cLib_addCalcPos2(cXyz* o_value, cXyz const& target, f32 scale, f32 maxStep);
void cLib_addCalcPosXZ2(cXyz* o_value, cXyz const& target, f32 scale, f32 maxStep);
s16 cLib_addCalcAngleS(s16* o_value, s16 target, s16 scale, s16 maxStep, s16 minStep);
void cLib_addCalcAngleS2(s16* o_value, s16 target, s16 scale, s16 maxStep);

int cLib_chaseUC(u8* o_value, u8 target, u8 step);
int cLib_chaseS(s16* o_value, s16 target, s16 step);
int cLib_chaseF(f32* o_value, f32 target, f32 step);
int cLib_chasePos(cXyz* o_value, cXyz const& target, f32 step);
int cLib_chasePosXZ(cXyz* o_value, cXyz const& target, f32 step);
int cLib_chaseAngleS(s16* o_value, s16 target, s16 step);

s16 cLib_targetAngleY(const Vec* lhs, const Vec* rhs);
s16 cLib_targetAngleY(const Vec& lhs, const Vec& rhs);
s16 cLib_targetAngleX(const cXyz*, const cXyz*);

void cLib_offsetPos(cXyz* pDest, cXyz const* pSrc, s16 angle, cXyz const* vec);
s32 cLib_distanceAngleS(s16 x, s16 y);

template <typename T>
inline void cLib_offBit(T& value, T bit) {
    value = (T)(value & ~bit);
}

template <typename T>
inline void cLib_onBit(T& value, T bit) {
    value = (T)(value | bit);
}

template <typename T>
inline T cLib_checkBit(T value, T bit) {
    return (T)(value & bit);
}

template <typename T>
inline T cLib_minMaxLimit(T val, T min, T max) {
    return (T)(val < min ? min : (val > max ? max : val));
}

template <typename T>
inline T cLib_maxLimit(T val, T max) {
    T ret;
    T var_r30;

    if (val > max) {
        var_r30 = max;
    } else {
        var_r30 = val;
    }

    ret = var_r30;
    return (T)ret;
}

template <typename T>
inline T cLib_minLimit(T val, T min) {
    T ret;

    if (val < min) {
        ret = min;
    } else {
        ret = val;
    }

    return (T)ret;
}

template <typename T>
T cLib_getRndValue(T min, T range) {
    return (T)(min + cM_rndF((f32)range));
}

template <typename T>
T cLib_calcTimer(T* value) {
    if (*(T*)value != 0) {
        --*value;
    }
    return *value;
}

void MtxInit(void);
void MtxTrans(f32, f32, f32, u8);
void MtxScale(f32, f32, f32, u8);
void MtxPosition(cXyz*, cXyz*);
void MtxPush(void);
void MtxPull(void);

extern Mtx* calc_mtx;

#endif
