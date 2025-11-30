#ifndef JPADYNAMICSBLOCK_H
#define JPADYNAMICSBLOCK_H

#include "JSystem/JGeometry.h"

#include "dolphin/types.h"

class JPAEmitterWorkData;

/**
 * @ingroup jsystem-jparticle
 * 
 */
struct JPADynamicsBlockData {
    // Common header.
    /* 0x00 */ u8 mMagic[4];
    /* 0x04 */ u32 mSize;

    /* 0x08 */ u32 mFlags;
    /* 0x0C */ u32 mResUserWork;
    /* 0x10 */ JGeometry::TVec3<f32> mEmitterScl;
    /* 0x1C */ JGeometry::TVec3<f32> mEmitterTrs;
    /* 0x28 */ JGeometry::TVec3<f32> mEmitterDir;
    /* 0x34 */ f32 mInitialVelOmni;
    /* 0x38 */ f32 mInitialVelAxis;
    /* 0x3C */ f32 mInitialVelRndm;
    /* 0x40 */ f32 mInitialVelDir;
    /* 0x44 */ f32 mSpread;
    /* 0x48 */ f32 mInitialVelRatio;
    /* 0x4C */ f32 mRate;
    /* 0x50 */ f32 mRateRndm;
    /* 0x54 */ f32 mLifeTimeRndm;
    /* 0x58 */ f32 mVolumeSweep;
    /* 0x5C */ f32 mVolumeMinRad;
    /* 0x60 */ f32 mAirResist;
    /* 0x64 */ f32 mMoment;
    /* 0x68 */ JGeometry::TVec3<s16> mEmitterRot;
    /* 0x6E */ s16 mMaxFrame;
    /* 0x70 */ s16 mStartFrame;
    /* 0x72 */ s16 mLifeTime;
    /* 0x74 */ u16 mVolumeSize;
    /* 0x76 */ u16 mDivNumber;
    /* 0x78 */ u8 mRateStep;
};  // Size: 0x7C

typedef void (*JPADynamicsCalcVolumeFunc)(JPAEmitterWorkData*);

enum {
    JPADynFlag_FixedDensity = 0x01,
    JPADynFlag_FixedInterval = 0x02,
    JPADynFlag_InheritScale = 0x04,
    JPADynFlag_FollowEmtr = 0x08,
    JPADynFlag_FollowEmtrChld = 0x10,
};

/**
 * @ingroup jsystem-jparticle
 * 
 */
class JPADynamicsBlock {
public:
    JPADynamicsBlock(u8 const*);
    void init();
    void create(JPAEmitterWorkData*);

    void calc(JPAEmitterWorkData* work) { mpCalcVolumeFunc(work); }

    s16 getStartFrame() const { return mpData->mStartFrame; }
    u32 getResUserWork() const { return mpData->mResUserWork; }
    u32 getFlag() const { return mpData->mFlags; }
    u32 getVolumeType() const { return (mpData->mFlags >> 8) & 0x07; }
    u16 getDivNumber() const { return mpData->mDivNumber; }
    f32 getRateRndm() const { return mpData->mRateRndm; }
    void getEmitterScl(JGeometry::TVec3<f32>* vec) const { 
        vec->set(mpData->mEmitterScl.x, mpData->mEmitterScl.y, mpData->mEmitterScl.z); 
    }
    void getEmitterTrs(JGeometry::TVec3<f32>* vec) const { 
        vec->set(mpData->mEmitterTrs.x, mpData->mEmitterTrs.y, mpData->mEmitterTrs.z); 
    }
    void getEmitterDir(JGeometry::TVec3<f32>* vec) const { 
        vec->set(mpData->mEmitterDir.x, mpData->mEmitterDir.y, mpData->mEmitterDir.z); 
    }
    void getEmitterRot(JGeometry::TVec3<s16>* vec) const { 
        vec->set(mpData->mEmitterRot.x, mpData->mEmitterRot.y, mpData->mEmitterRot.z); 
    } 
    s16 getMaxFrame() { return mpData->mMaxFrame; }
    s16 getLifetime() { return mpData->mLifeTime; }
    u16 getVolumeSize() { return mpData->mVolumeSize; }
    f32 getRate() { return mpData->mRate; }
    u8 getRateStep() { return mpData->mRateStep; }
    f32 getVolumeSweep() { return mpData->mVolumeSweep; }
    f32 getVolumeMinRad() { return mpData->mVolumeMinRad; }
    f32 getInitVelOmni() { return mpData->mInitialVelOmni; }
    f32 getInitVelAxis() { return mpData->mInitialVelAxis; }
    f32 getInitVelDir() { return mpData->mInitialVelDir; }
    f32 getInitVelDirSp() { return mpData->mSpread; }
    f32 getInitVelRndm() { return mpData->mInitialVelRndm; }
    f32 getInitVelRatio() const { return mpData->mInitialVelRatio; }
    f32 getAirRes() { return mpData->mAirResist; }
    f32 getLifetimeRndm() const { return mpData->mLifeTimeRndm; }
    f32 getMomentRndm() const { return mpData->mMoment; }

public:
    /* 0x00 */ const JPADynamicsBlockData* mpData;
    /* 0x04 */ JPADynamicsCalcVolumeFunc mpCalcVolumeFunc;
};

#endif /* JPADYNAMICSBLOCK_H */
