#ifndef J3DSTRUCT_H
#define J3DSTRUCT_H

#include <dolphin/gx.h>
#include <dolphin/mtx.h>
#include <dolphin/mtx.h>
#include "global.h"

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DLightInfo {
    bool operator==(J3DLightInfo& other) const;
    J3DLightInfo& operator=(J3DLightInfo const&);

    /* 0x00 */ Vec mLightPosition;
    /* 0x0C */ Vec mLightDirection;
    /* 0x18 */ GXColor mColor;
    /* 0x1C */ Vec mCosAtten;
    /* 0x28 */ Vec mDistAtten;
};  // Size = 0x34

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTextureSRTInfo {
    /* 0x00 */ f32 mScaleX;
    /* 0x04 */ f32 mScaleY;
    /* 0x08 */ s16 mRotation;
    /* 0x0C */ f32 mTranslationX;
    /* 0x10 */ f32 mTranslationY;

    bool operator==(J3DTextureSRTInfo&) const;
    inline void operator=(J3DTextureSRTInfo const& other) {
#ifdef __MWERKS__
        __REGISTER const f32* src = &other.mScaleX;
        __REGISTER f32* dst = &mScaleX;
        __REGISTER f32 xy;
        asm {
            psq_l xy, 0(src), 0, 0
            psq_st xy, 0(dst), 0, 0
        };

        // Unclear why there's a 4 byte copy here.
        *(u32*)&mRotation = *(u32*)&other.mRotation;
        src = &other.mTranslationX;
        dst = &mTranslationX;

        asm {
            psq_l xy, 0(src), 0, 0
            psq_st xy, 0(dst), 0, 0
        };
#endif
    }
};  // Size: 0x14

enum J3DTexMtxMode {
    J3DTexMtxMode_None,
    J3DTexMtxMode_EnvmapBasic,
    J3DTexMtxMode_ProjmapBasic,
    J3DTexMtxMode_ViewProjmapBasic,
    J3DTexMtxMode_Unknown4,
    J3DTexMtxMode_Unknown5,
    J3DTexMtxMode_EnvmapOld,
    J3DTexMtxMode_Envmap,
    J3DTexMtxMode_Projmap,
    J3DTexMtxMode_ViewProjmap,
    J3DTexMtxMode_EnvmapOldEffectMtx,
    J3DTexMtxMode_EnvmapEffectMtx,
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTexMtxInfo {
    bool operator==(J3DTexMtxInfo& other) const;
    J3DTexMtxInfo& operator=(J3DTexMtxInfo const&);
    void setEffectMtx(Mtx);

    /* 0x00 */ u8 mProjection;
    /* 0x01 */ u8 mInfo;
    /* 0x02 */ u8 field_0x2;
    /* 0x03 */ u8 field_0x3;
    /* 0x04 */ Vec mCenter;
    /* 0x10 */ J3DTextureSRTInfo mSRT;
    /* 0x24 */ Mtx44 mEffectMtx;    
};  // Size: 0x64

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DIndTexMtxInfo {
    J3DIndTexMtxInfo& operator=(J3DIndTexMtxInfo const&);
    /* 0x00 */ Mtx23 field_0x0;
    /* 0x18 */ s8 field_0x18;
};  // Size: 0x1C

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DFogInfo {
    bool operator==(J3DFogInfo&) const;
    J3DFogInfo& operator=(const J3DFogInfo&);

    /* 0x00 */ u8 mType;
    /* 0x01 */ u8 mAdjEnable;
    /* 0x02 */ u16 mCenter;
    /* 0x04 */ f32 mStartZ;
    /* 0x08 */ f32 mEndZ;
    /* 0x0C */ f32 mNearZ;
    /* 0x10 */ f32 mFarZ;
    /* 0x14 */ GXColor mColor;
    /* 0x18 */ GXFogAdjTable mFogAdjTable;
};  // Size: 0x2C

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DNBTScaleInfo {
    bool operator==(const J3DNBTScaleInfo& other) const;
    J3DNBTScaleInfo& operator=(const J3DNBTScaleInfo&);

    /* 0x0 */ u8 mbHasScale;
    /* 0x4 */ Vec mScale;
};  // Size: 0x10

/**
 * @ingroup jsystem-j3d
 *
 */
struct J3DIndTexOrderInfo {
    /* 0x0 */ u8 mCoord;
    /* 0x1 */ u8 mMap;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;

    void operator=(J3DIndTexOrderInfo const& other) {
        __memcpy(this, &other, sizeof(J3DIndTexOrderInfo));
    }
};  // Size: 0x04

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTevSwapModeInfo {
    /* 0x0 */ u8 mRasSel;
    /* 0x1 */ u8 mTexSel;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;
}; // Size: 0x4

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTevSwapModeTableInfo {
    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 field_0x1;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;
}; // Size: 0x4

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTevStageInfo {
    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 mTevColorOp;
    /* 0x2 */ u8 mTevColorAB;
    /* 0x3 */ u8 mTevColorCD;
    /* 0x4 */ u8 field_0x4;
    /* 0x5 */ u8 mTevAlphaOp;
    /* 0x6 */ u8 mTevAlphaAB;
    /* 0x7 */ u8 mTevSwapModeInfo;
    /* 0x8 */ u8 field_0x8;
    /* 0x8 */ u8 field_0x9;
    /* 0x8 */ u8 field_0xa;
    /* 0x8 */ u8 field_0xb;
    /* 0x8 */ u8 field_0xc;
    /* 0x8 */ u8 field_0xd;
    /* 0x8 */ u8 field_0xe;
    /* 0x8 */ u8 field_0xf;
    /* 0x8 */ u8 field_0x10;
    /* 0x8 */ u8 field_0x11;
    /* 0x8 */ u8 field_0x12;
    /* 0x8 */ u8 field_0x13;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DIndTevStageInfo {
    /* 0x0 */ u8 mIndStage;
    /* 0x1 */ u8 mIndFormat;
    /* 0x2 */ u8 mBiasSel;
    /* 0x3 */ u8 mMtxSel;
    /* 0x4 */ u8 mWrapS;
    /* 0x5 */ u8 mWrapT;
    /* 0x6 */ u8 mPrev;
    /* 0x7 */ u8 mLod;
    /* 0x8 */ u8 mAlphaSel;
    /* 0x9 */ u8 pad[3];
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTexCoordInfo {
    /* 0x0 */ u8 mTexGenType;
    /* 0x1 */ u8 mTexGenSrc;
    /* 0x2 */ u8 mTexGenMtx;
    /* 0x3 */ u8 pad;

    J3DTexCoordInfo& operator=(const J3DTexCoordInfo& other) {
        __memcpy(this, &other, sizeof(J3DTexCoordInfo));
        return *this;
    }
};

/**
 * @ingroup jsystem-j3d
 *
 */
struct J3DIndTexCoordScaleInfo {
    /* 0x0 */ u8 mScaleS;
    /* 0x1 */ u8 mScaleT;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 field_0x3;

    J3DIndTexCoordScaleInfo& operator=(const J3DIndTexCoordScaleInfo& other) {
        __memcpy(this, &other, sizeof(J3DIndTexCoordScaleInfo));
        return *this;
    }
};  // Size: 0x4

/**
 * @ingroup jsystem-j3d
 *
 */
struct J3DBlendInfo {
    void operator=(J3DBlendInfo const& other) {
        __memcpy(this, &other, sizeof(J3DBlendInfo));
    }
    /* 0x0 */ u8 mType;
    /* 0x1 */ u8 mSrcFactor;
    /* 0x2 */ u8 mDstFactor;
    /* 0x3 */ u8 mOp;
};

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DTevOrderInfo {
    void operator=(const J3DTevOrderInfo& other) {
        *(u32*) this = *(u32*)&other;
    }

    /* 0x0 */ u8 mTexCoord;
    /* 0x1 */ u8 mTexMap;
    /* 0x2 */ u8 mColorChan;
    /* 0x3 */ u8 field_0x3; // Maybe padding
};

/**
 * @ingroup jsystem-j3d
 *
 */
struct J3DColorChanInfo {
    /* 0x0 */ u8 mEnable;
    /* 0x1 */ u8 mMatSrc;
    /* 0x2 */ u8 mLightMask;
    /* 0x3 */ u8 mDiffuseFn;
    /* 0x4 */ u8 mAttnFn;
    /* 0x5 */ u8 mAmbSrc;
    /* 0x6 */ u8 pad[2];
};

/**
 * @ingroup jsystem-j3d
 *
 */
struct J3DZModeInfo {
    /* 0x0 */ u8 field_0x0;
    /* 0x1 */ u8 field_0x1;
    /* 0x2 */ u8 field_0x2;
    /* 0x3 */ u8 pad;
};

/**
 * @ingroup jsystem-j3d
 *
 */
struct J3DAlphaCompInfo {
    /* 0x0 */ u8 mComp0;
    /* 0x1 */ u8 mRef0;
    /* 0x2 */ u8 mOp;
    /* 0x3 */ u8 mComp1;
    /* 0x4 */ u8 mRef1;
    /* 0x5 */ u8 field_0x5;
    /* 0x6 */ u8 field_0x6;
    /* 0x7 */ u8 field_0x7;

    J3DAlphaCompInfo& operator=(const J3DAlphaCompInfo& other) {
        mComp0 = other.mComp0;
        mRef0 = other.mRef0;
        mOp = other.mOp;
        mComp1 = other.mComp1;
        mRef1 = other.mRef1;
        return *this;
    }
};

#endif /* J3DSTRUCT_H */
