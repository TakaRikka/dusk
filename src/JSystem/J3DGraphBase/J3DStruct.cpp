//
// J3DStruct
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DStruct.h"
#include "JSystem/JMath/JMath.h"

bool J3DTextureSRTInfo::operator==(J3DTextureSRTInfo& other) const {
    if (mScaleX == other.mScaleX &&
        mScaleY == other.mScaleY &&
        mRotation == other.mRotation &&
        mTranslationX == other.mTranslationX &&
        mTranslationY == other.mTranslationY
    ) {
        return true;
    }
    return false;
}

bool J3DLightInfo::operator==(J3DLightInfo& other) const {
    if (mLightPosition.x == other.mLightPosition.x &&
        mLightPosition.y == other.mLightPosition.y &&
        mLightPosition.z == other.mLightPosition.z &&
        mLightDirection.x == other.mLightDirection.x &&
        mLightDirection.y == other.mLightDirection.y &&
        mLightDirection.z == other.mLightDirection.z &&
        mColor.r == other.mColor.r &&
        mColor.g == other.mColor.g &&
        mColor.b == other.mColor.b &&
        mColor.a == other.mColor.a &&
        mCosAtten.x == other.mCosAtten.x &&
        mCosAtten.y == other.mCosAtten.y &&
        mCosAtten.z == other.mCosAtten.z &&
        mDistAtten.x == other.mDistAtten.x &&
        mDistAtten.y == other.mDistAtten.y &&
        mDistAtten.z == other.mDistAtten.z
    ) {
        return true;
    }
    return false;
}

J3DLightInfo& J3DLightInfo::operator=(J3DLightInfo const& param_0) {
    JMath::gekko_ps_copy6(&mLightPosition, &param_0.mLightPosition);
    mColor = param_0.mColor;
    JMath::gekko_ps_copy6(&mCosAtten, &param_0.mCosAtten);
    return *this;
}

bool J3DTexMtxInfo::operator==(J3DTexMtxInfo& other) const {
    if (mProjection != other.mProjection ||
        mInfo != other.mInfo ||
        !(mSRT == other.mSRT)
    ) {
        return false;
    }
    if (mEffectMtx[0][0] != other.mEffectMtx[0][0] ||
        mEffectMtx[0][1] != other.mEffectMtx[0][1] ||
        mEffectMtx[0][2] != other.mEffectMtx[0][2] ||
        mEffectMtx[0][3] != other.mEffectMtx[0][3] ||
        mEffectMtx[1][0] != other.mEffectMtx[1][0] ||
        mEffectMtx[1][1] != other.mEffectMtx[1][1] ||
        mEffectMtx[1][2] != other.mEffectMtx[1][2] ||
        mEffectMtx[1][3] != other.mEffectMtx[1][3] ||
        mEffectMtx[2][0] != other.mEffectMtx[2][0] ||
        mEffectMtx[2][1] != other.mEffectMtx[2][1] ||
        mEffectMtx[2][2] != other.mEffectMtx[2][2] ||
        mEffectMtx[2][3] != other.mEffectMtx[2][3] ||
        mEffectMtx[3][0] != other.mEffectMtx[3][0] ||
        mEffectMtx[3][1] != other.mEffectMtx[3][1] ||
        mEffectMtx[3][2] != other.mEffectMtx[3][2] ||
        mEffectMtx[3][3] != other.mEffectMtx[3][3]
    ) {
        return false;
    }
    if (
        mCenter.x != other.mCenter.x ||
        mCenter.y != other.mCenter.y ||
        mCenter.z != other.mCenter.z
    ) {
        return false;
    }
    return true;
}

J3DTexMtxInfo& J3DTexMtxInfo::operator=(J3DTexMtxInfo const& param_0) {
    mProjection = param_0.mProjection;
    mInfo = param_0.mInfo;
    JMath::gekko_ps_copy3(&mCenter, &param_0.mCenter);
    mSRT = param_0.mSRT;
    JMath::gekko_ps_copy16(&mEffectMtx, &param_0.mEffectMtx);
    return *this;
}

// Helper functions to set float literal order
static f32 dummyLiteralOrder() {
    return 1.0f;
}

void J3DTexMtxInfo::setEffectMtx(Mtx param_0) {
#ifdef __MWERKS__
    JMath::gekko_ps_copy12(&mEffectMtx, param_0);
    register f32 zero = 0.0f;
    register f32* mtx = (f32*)mEffectMtx;
    asm {
        psq_st zero, 0x30(mtx), 0, 0
    };
    mEffectMtx[3][2] = 0.0f;
    mEffectMtx[3][3] = 1.0f;
#endif
}

J3DIndTexMtxInfo& J3DIndTexMtxInfo::operator=(J3DIndTexMtxInfo const& param_0) {
    JMath::gekko_ps_copy6(field_0x0, param_0.field_0x0);
    field_0x18 = param_0.field_0x18;
    return *this;
}

bool J3DFogInfo::operator==(J3DFogInfo& other) const {
    if (mType == other.mType &&
        mAdjEnable == other.mAdjEnable &&
        mCenter == other.mCenter &&
        mStartZ == other.mStartZ &&
        mEndZ == other.mEndZ &&
        mNearZ == other.mNearZ &&
        mFarZ == other.mFarZ &&
        mColor.r == other.mColor.r &&
        mColor.g == other.mColor.g &&
        mColor.b == other.mColor.b &&
        mColor.a == other.mColor.a
    ) {
        for (int i = 0; i < 10; i++) {
            if (mFogAdjTable.r[i] != other.mFogAdjTable.r[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

J3DFogInfo& J3DFogInfo::operator=(const J3DFogInfo& param_0) {
    mType = param_0.mType;
    mAdjEnable = param_0.mAdjEnable;
    mCenter = param_0.mCenter;
    mStartZ = param_0.mStartZ;
    mEndZ = param_0.mEndZ;
    mNearZ = param_0.mNearZ;
    mFarZ = param_0.mFarZ;
    mColor.r = param_0.mColor.r;
    mColor.g = param_0.mColor.g;
    mColor.b = param_0.mColor.b;
    mColor.a = param_0.mColor.a;
    for (int i = 0; i < 10; i++) {
        mFogAdjTable.r[i] = param_0.mFogAdjTable.r[i];
    }
    return *this;
}

bool J3DNBTScaleInfo::operator==(const J3DNBTScaleInfo& other) const {
    if (mbHasScale == other.mbHasScale &&
        mScale.x == other.mScale.x &&
        mScale.y == other.mScale.y &&
        mScale.z == other.mScale.z
    ) {
        return true;
    }
    return false;
}

J3DNBTScaleInfo& J3DNBTScaleInfo::operator=(const J3DNBTScaleInfo& other) {
    mbHasScale = other.mbHasScale;
    mScale.x = other.mScale.x;
    mScale.y = other.mScale.y;
    mScale.z = other.mScale.z;
    return *this;
}
