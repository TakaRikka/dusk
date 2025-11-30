//
// J3DStruct
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DStruct.h"
#include "JSystem/JMath/JMath.h"

J3DLightInfo& J3DLightInfo::operator=(J3DLightInfo const& param_0) {
    JMath::gekko_ps_copy6(&mLightPosition, &param_0.mLightPosition);
    mColor = param_0.mColor;
    JMath::gekko_ps_copy6(&mCosAtten, &param_0.mCosAtten);
    return *this;
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

J3DFogInfo& J3DFogInfo::operator=(J3DFogInfo const& param_0) {
    mType = param_0.mType;
    mAdjEnable = param_0.mAdjEnable;
    mCenter = param_0.mCenter;
    mStartZ = param_0.mStartZ;
    mEndZ = param_0.mEndZ;
    mNearZ = param_0.mNearZ;
    mFarZ = param_0.mFarZ;
    mColor = param_0.mColor;
    for (int i = 0; i < 10; i++) {
        mFogAdjTable.r[i] = param_0.mFogAdjTable.r[i];
    }
    return *this;
}

J3DNBTScaleInfo& J3DNBTScaleInfo::operator=(J3DNBTScaleInfo const& param_0) {
    mbHasScale = param_0.mbHasScale;
    mScale = param_0.mScale;
    return *this;
}
