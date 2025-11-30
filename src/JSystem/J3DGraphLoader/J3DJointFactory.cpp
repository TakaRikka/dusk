#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphLoader/J3DJointFactory.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/JSupport/JSupport.h"

J3DJointFactory::J3DJointFactory(J3DJointBlock const& block) {
    mJointInitData = JSUConvertOffsetToPtr<J3DJointInitData>(&block, (uintptr_t)block.mpJointInitData);
    mIndexTable = JSUConvertOffsetToPtr<u16>(&block, (uintptr_t)block.mpIndexTable);
}

J3DJoint* J3DJointFactory::create(int no) {
    J3DJoint* joint = new J3DJoint();
    joint->mJntNo = no;
    joint->mKind = getKind(no);
    joint->mScaleCompensate = getScaleCompensate(no);
    joint->mTransformInfo = getTransformInfo(no);
    joint->mBoundingSphereRadius = getRadius(no);
    joint->mMin = getMin(no);
    joint->mMax = getMax(no);
    joint->mMtxCalc = NULL;

    if (joint->mScaleCompensate == 0xFF)
        joint->mScaleCompensate = 0;

    return joint;
}
