#ifndef J3DJOINTFACTORY_H
#define J3DJOINTFACTORY_H

#include "JSystem/J3DGraphBase/J3DTransform.h"

class J3DJoint;
struct J3DJointBlock;

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DJointInitData {
    /* 0x00 */ u16 mKind;
    /* 0x02 */ bool mScaleCompensate;
    /* 0x04 */ J3DTransformInfo mTransformInfo;
    /* 0x24 */ f32 mRadius;
    /* 0x28 */ Vec mMin;
    /* 0x2C */ Vec mMax;
};  // Size: 0x30

/**
 * @ingroup jsystem-j3d
 * 
 */
struct J3DJointFactory {
    J3DJointFactory(J3DJointBlock const&);
    J3DJoint* create(int);

    J3DJointInitData* mJointInitData;
    u16* mIndexTable;

    u16 getKind(int no) const { return mJointInitData[mIndexTable[no]].mKind; }
    u8 getScaleCompensate(int no) const { return mJointInitData[mIndexTable[no]].mScaleCompensate; }
    const J3DTransformInfo& getTransformInfo(int no) const {
        return mJointInitData[mIndexTable[no]].mTransformInfo;
    }
    f32 getRadius(int no) const { return mJointInitData[mIndexTable[no]].mRadius; }
    Vec& getMin(int no) const { return mJointInitData[mIndexTable[no]].mMin; }
    Vec& getMax(int no) const { return mJointInitData[mIndexTable[no]].mMax; }
};

#endif /* J3DJOINTFACTORY_H */
