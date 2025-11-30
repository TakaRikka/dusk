#ifndef JPACHILDSHAPE_H
#define JPACHILDSHAPE_H

#include <dolphin/gx.h>

class JPAEmitterWorkData;
class JPABaseParticle;

/**
 * @ingroup jsystem-jparticle
 * 
 */
struct JPAChildShapeData {
    // Common header.
    /* 0x00 */ u8 mMagic[4];
    /* 0x04 */ u32 mSize;

    /* 0x08 */ u32 mFlags;
    /* 0x0C */ f32 mPosRndm;
    /* 0x10 */ f32 mBaseVel;
    /* 0x14 */ f32 mBaseVelRndm;
    /* 0x18 */ f32 mVelInfRate;
    /* 0x1C */ f32 mGravity;
    /* 0x20 */ f32 mScaleX;
    /* 0x24 */ f32 mScaleY;
    /* 0x28 */ f32 mInheritScale;
    /* 0x2C */ f32 mInheritAlpha;
    /* 0x30 */ f32 mInheritRGB;
    /* 0x34 */ GXColor mPrmClr;
    /* 0x38 */ GXColor mEnvClr;
    /* 0x3C */ f32 mTiming;
    /* 0x40 */ s16 mLife;
    /* 0x42 */ s16 mRate;
    /* 0x44 */ u8 mStep;
    /* 0x45 */ u8 mTexIdx;
    /* 0x46 */ s16 mRotSpeed;
};  // Size: 0x48

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPAChildShape {
public:
    JPAChildShape(u8 const*);

    f32 getPosRndm() const { return mpData->mPosRndm; }
    f32 getBaseVel() const { return mpData->mBaseVel; }
    f32 getBaseVelRndm() const { return mpData->mBaseVelRndm; }
    f32 getVelInhRate() const { return mpData->mVelInfRate; }
    f32 getGravity() const { return mpData->mGravity; }
    f32 getScaleX() const { return mpData->mScaleX; }
    f32 getScaleY() const { return mpData->mScaleY; }
    f32 getScaleInhRate() const { return mpData->mInheritScale; }
    f32 getAlphaInhRate() const { return mpData->mInheritAlpha; }
    f32 getColorInhRate() const { return mpData->mInheritRGB; }
    void getPrmClr(GXColor* dst) const { *dst = mpData->mPrmClr; }
    u8 getPrmAlpha() const { return mpData->mPrmClr.a; }
    void getEnvClr(GXColor* dst) const { *dst = mpData->mEnvClr; }
    f32 getTiming() const { return mpData->mTiming; }
    s16 getLife() const { return mpData->mLife; }
    s16 getRate() const { return mpData->mRate; }
    u8 getStep() const { return mpData->mStep; }
    u8 getTexIdx() const { return mpData->mTexIdx; }
    s16 getRotInitSpeed() const { return mpData->mRotSpeed; }
    u32 getType() const { return mpData->mFlags & 0xF; }
    u32 getDirType() const { return (mpData->mFlags >> 4) & 7; }
    u32 getRotType() const { return (mpData->mFlags >> 7) & 7; }
    u32 getBasePlaneType() const { return (mpData->mFlags >> 10) & 1; }

    BOOL isScaleInherited() const { return mpData->mFlags & 0x10000; }
    BOOL isAlphaInherited() const { return mpData->mFlags & 0x20000; }
    BOOL isColorInherited() const { return mpData->mFlags & 0x40000; }
    BOOL isClipOn() const { return mpData->mFlags & 0x100000; }
    BOOL isFieldAffected() const { return mpData->mFlags & 0x200000; }
    BOOL isScaleOutOn() const { return mpData->mFlags & 0x400000; }
    BOOL isAlphaOutOn() const { return mpData->mFlags & 0x800000; }
    BOOL isRotateOn() const { return mpData->mFlags & 0x1000000; }

public:
    /* 0x00 */ const JPAChildShapeData* mpData;
};

void JPARegistChildPrmEnv(JPAEmitterWorkData*);

void JPACalcChildAlphaOut(JPAEmitterWorkData*, JPABaseParticle*);
void JPACalcChildScaleOut(JPAEmitterWorkData*, JPABaseParticle*);

#endif /* JPACHILDSHAPE_H */
