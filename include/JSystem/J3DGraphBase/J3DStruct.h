#ifndef J3DSTRUCT_H
#define J3DSTRUCT_H

#include <dolphin/gx.h>
#include "dolphin/mtx.h"
#include "dolphin/mtx.h"

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DLightInfo {
public:
    J3DLightInfo& operator=(J3DLightInfo const&);

    /* 0x00 */ Vec mLightPosition;
    /* 0x0C */ Vec mLightDirection;
    /* 0x18 */ GXColor mColor;
    /* 0x1C */ Vec mCosAtten;
    /* 0x28 */ Vec mDistAtten;
};  // Size = 0x34

extern "C" extern J3DLightInfo const j3dDefaultLightInfo;

/**
 * @ingroup jsystem-j3d
 * 
 */
class J3DLightObj {
public:
    J3DLightObj() { mInfo = j3dDefaultLightInfo; }
    void load(u32) const;

    J3DLightInfo* getLightInfo() { return &mInfo; }
    J3DLightObj& operator=(J3DLightObj const& other) {
        mInfo = other.mInfo;
        return *this;
    }

    /* 0x00 */ J3DLightInfo mInfo;
    /* 0x34 */ GXLightObj mLightObj;
};  // Size = 0x74

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
    /* 0x18 */ u8 field_0x18;
};  // Size: 0x1C

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DFogInfo {
    J3DFogInfo& operator=(J3DFogInfo const&);

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
    J3DNBTScaleInfo& operator=(J3DNBTScaleInfo const&);

    /* 0x0 */ u8 mbHasScale;
    /* 0x4 */ Vec mScale;
};  // Size: 0x10

#endif /* J3DSTRUCT_H */
