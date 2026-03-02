#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphLoader/J3DJointFactory.h"
#include "JSystem/J3DGraphLoader/J3DModelLoader.h"
#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/JSupport/JSupport.h"

J3DJointFactory::J3DJointFactory(J3DJointBlock const& block) {
    mJointInitData = JSUConvertOffsetToPtr<J3DJointInitData>(&block, (uintptr_t)block.mpJointInitData);
    mIndexTable = JSUConvertOffsetToPtr<u16>(&block, (uintptr_t)block.mpIndexTable);

#if TARGET_LITTLE_ENDIAN
    for (int i = 0; i < block.mJointNum; i++) {
        auto& index = mIndexTable[i];
        be_swap(index);

        auto initData = &mJointInitData[index];
        be_swap(initData->mKind);
        be_swap(initData->mTransformInfo.mScale);
        be_swap(initData->mTransformInfo.mRotation);
        be_swap(initData->mTransformInfo.mTranslate);
        be_swap(initData->mRadius);
        be_swap(initData->mMin);
        be_swap(initData->mMax);
    }
#endif
}

J3DJoint* J3DJointFactory::create(int no) {
    J3DJoint* joint = new J3DJoint();
    J3D_ASSERT_ALLOCMEM(50, joint);
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
