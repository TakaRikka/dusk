#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphAnimator/J3DJoint.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"
#include "JSystem/J3DGraphBase/J3DDrawBuffer.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/JMath/JMath.h"
#include "m_Do/m_Do_mtx.h"

void J3DMtxCalcJ3DSysInitBasic::init(Vec const& scale, Mtx const& mtx) {
    J3DSys::mCurrentS = scale;
    Vec init = {1.0f, 1.0f, 1.0f};
    J3DSys::mParentS = init;
    JMAMTXApplyScale(mtx, J3DSys::mCurrentMtx, scale.x, scale.y, scale.z);
}

void J3DMtxCalcJ3DSysInitMaya::init(Vec const& scale, Mtx const& mtx) {
    Vec init = {1.0f, 1.0f, 1.0f};
    J3DSys::mParentS = init;
    J3DSys::mCurrentS = scale;
    JMAMTXApplyScale(mtx, J3DSys::mCurrentMtx, scale.x, scale.y, scale.z);
}

J3DMtxBuffer* J3DMtxCalc::mMtxBuffer;

J3DJoint* J3DMtxCalc::mJoint;

inline s32 checkScaleOne(const Vec& param_0) {
    if (param_0.x == 1.0f && param_0.y == 1.0f && param_0.z == 1.0f) {
        return true;
    } else {
        return false;
    }
}

void J3DMtxCalcCalcTransformBasic::calcTransform(J3DTransformInfo const& transInfo) {
    J3DJoint* joint = J3DMtxCalc::getJoint();
    J3DMtxBuffer* mtxBuf = J3DMtxCalc::getMtxBuffer();
    u16 jntNo = joint->getJntNo();

    MtxP anmMtx = mtxBuf->getAnmMtx(jntNo);

    J3DSys::mCurrentS.x *= transInfo.mScale.x;
    J3DSys::mCurrentS.y *= transInfo.mScale.y;
    J3DSys::mCurrentS.z *= transInfo.mScale.z;
    J3DGetTranslateRotateMtx(transInfo, anmMtx);

    if (!checkScaleOne(J3DSys::mCurrentS)) {
        mtxBuf->setScaleFlag(jntNo, 0);
        JMAMTXApplyScale(anmMtx, anmMtx, transInfo.mScale.x, transInfo.mScale.y,
                         transInfo.mScale.z);
    } else {
        mtxBuf->setScaleFlag(jntNo, 1);
    }

    MTXConcat(J3DSys::mCurrentMtx, anmMtx, J3DSys::mCurrentMtx);
    MTXCopy(J3DSys::mCurrentMtx, anmMtx);
}

void J3DMtxCalcCalcTransformSoftimage::calcTransform(J3DTransformInfo const& transInfo) {
    J3DJoint* joint = J3DMtxCalc::getJoint();
    J3DMtxBuffer* mtxBuf = J3DMtxCalc::getMtxBuffer();
    u16 jntNo = joint->getJntNo();

    MtxP anmMtx = mtxBuf->getAnmMtx(jntNo);

    J3DGetTranslateRotateMtx(transInfo.mRotation.x, transInfo.mRotation.y, transInfo.mRotation.z,
                             transInfo.mTranslate.x * J3DSys::mCurrentS.x,
                             transInfo.mTranslate.y * J3DSys::mCurrentS.y,
                             transInfo.mTranslate.z * J3DSys::mCurrentS.z, anmMtx);
    MTXConcat(J3DSys::mCurrentMtx, anmMtx, J3DSys::mCurrentMtx);

    J3DSys::mCurrentS.x *= transInfo.mScale.x;
    J3DSys::mCurrentS.y *= transInfo.mScale.y;
    J3DSys::mCurrentS.z *= transInfo.mScale.z;

    if (!checkScaleOne(J3DSys::mCurrentS)) {
        mtxBuf->setScaleFlag(jntNo, 0);
        JMAMTXApplyScale(J3DSys::mCurrentMtx, anmMtx, J3DSys::mCurrentS.x, J3DSys::mCurrentS.y,
                         J3DSys::mCurrentS.z);
        anmMtx[0][3] = J3DSys::mCurrentMtx[0][3];
        anmMtx[1][3] = J3DSys::mCurrentMtx[1][3];
        anmMtx[2][3] = J3DSys::mCurrentMtx[2][3];
    } else {
        mtxBuf->setScaleFlag(jntNo, 1);
        MTXCopy(J3DSys::mCurrentMtx, anmMtx);
    }
}

void J3DMtxCalcCalcTransformMaya::calcTransform(J3DTransformInfo const& transInfo) {
    J3DJoint* joint = J3DMtxCalc::getJoint();
    J3DMtxBuffer* mtxBuf = J3DMtxCalc::getMtxBuffer();

    u16 jntNo = joint->getJntNo();

    MtxP anmMtx = mtxBuf->getAnmMtx(jntNo);

    J3DGetTranslateRotateMtx(transInfo, anmMtx);

    if (transInfo.mScale.x == 1.0f && transInfo.mScale.y == 1.0f && transInfo.mScale.z == 1.0f) {
        mtxBuf->setScaleFlag(jntNo, 1);
    } else {
        mtxBuf->setScaleFlag(jntNo, 0);
        JMAMTXApplyScale(anmMtx, anmMtx, transInfo.mScale.x, transInfo.mScale.y,
                         transInfo.mScale.z);
    }

    u8 scaleCompensate = joint->getScaleCompensate();
    if (scaleCompensate == 1) {
        Vec inv;
        inv.x = JMath::fastReciprocal(J3DSys::mParentS.x);
        inv.y = JMath::fastReciprocal(J3DSys::mParentS.y);
        inv.z = JMath::fastReciprocal(J3DSys::mParentS.z);

        anmMtx[0][0] *= inv.x;
        anmMtx[0][1] *= inv.x;
        anmMtx[0][2] *= inv.x;
        anmMtx[1][0] *= inv.y;
        anmMtx[1][1] *= inv.y;
        anmMtx[1][2] *= inv.y;
        anmMtx[2][0] *= inv.z;
        anmMtx[2][1] *= inv.z;
        anmMtx[2][2] *= inv.z;
    }

    MTXConcat(J3DSys::mCurrentMtx, anmMtx, J3DSys::mCurrentMtx);
    MTXCopy(J3DSys::mCurrentMtx, anmMtx);

    J3DSys::mParentS.x = transInfo.mScale.x;
    J3DSys::mParentS.y = transInfo.mScale.y;
    J3DSys::mParentS.z = transInfo.mScale.z;
}

void J3DJoint::appendChild(J3DJoint* pChild) {
    if (mChild == NULL) {
        mChild = pChild;
        return;
    }
    J3DJoint* curChild = mChild;
    while (curChild->getYounger() != NULL) {
        curChild = curChild->getYounger();
    }
    curChild->setYounger(pChild);
}

J3DJoint::J3DJoint() {
    mCallBackUserData = NULL;
    mCallBack = NULL;
    field_0x8 = NULL;
    mChild = NULL;
    mYounger = NULL;
    mJntNo = 0;
    mKind = 1;
    mScaleCompensate = false;
    J3DTransformInfo* r30 = &mTransformInfo;
    void* r29 = __memcpy(r30, &j3dDefaultTransformInfo, sizeof(J3DTransformInfo));
    mBoundingSphereRadius = 0.0f;
    mMtxCalc = NULL;
    mMesh = NULL;

    Vec init = {0.0f, 0.0f, 0.0f};
    mMin = init;
    Vec init2 = {0.0f, 0.0f, 0.0f};
    mMax = init2;
}

void J3DJoint::entryIn() {
    MtxP anmMtx = j3dSys.getModel()->getAnmMtx(mJntNo);
    j3dSys.getDrawBuffer(0)->setZMtx(anmMtx);
    j3dSys.getDrawBuffer(1)->setZMtx(anmMtx);
    for (J3DMaterial* mesh = mMesh; mesh != NULL;) {
        if (mesh->getShape()->checkFlag(J3DShpFlag_Visible)) {
            mesh = mesh->getNext();
        } else {
            J3DMatPacket* matPacket = j3dSys.getModel()->getMatPacket(mesh->getIndex());
            J3DShapePacket* shapePacket =
                j3dSys.getModel()->getShapePacket(mesh->getShape()->getIndex());
            if (!matPacket->isLocked()) {
                if (mesh->getMaterialAnm()) {
                    mesh->getMaterialAnm()->calc(mesh);
                }
                mesh->calc(anmMtx);
            }
            mesh->setCurrentMtx();
            matPacket->setMaterialAnmID(mesh->getMaterialAnm());
            matPacket->setShapePacket(shapePacket);
            bool isDrawModeOpaTexEdge = mesh->isDrawModeOpaTexEdge() == FALSE;
            u8 r24 = matPacket->entry(j3dSys.getDrawBuffer(isDrawModeOpaTexEdge));
            if (r24) {
                j3dSys.setMatPacket(matPacket);
                J3DDrawBuffer::entryNum++;
                mesh->makeDisplayList();
            }
            mesh = mesh->getNext();
        }
    }
}

J3DMtxCalc* J3DJoint::mCurrentMtxCalc;

void J3DJoint::recursiveCalc() {
    J3DMtxCalc* prevMtxCalc = NULL;
    Mtx prevCurrentMtx;
    MTXCopy(J3DSys::mCurrentMtx, prevCurrentMtx);
    Vec current, parent;
    current = J3DSys::mCurrentS;
    parent = J3DSys::mParentS;
    if (getMtxCalc() != NULL) {
        prevMtxCalc = getCurrentMtxCalc();
        J3DMtxCalc* piVar2 = this->getMtxCalc();
        setCurrentMtxCalc(piVar2);
        J3DMtxCalc::setJoint(this);
#if DEBUG
        J3DMtxCalc::setMtxBuffer(J3DMtxCalc::getMtxBuffer());
#endif
        piVar2->calc();
    } else {
        if (getCurrentMtxCalc() != NULL) {
            J3DMtxCalc* uVar6 = getCurrentMtxCalc();
            J3DMtxCalc::setJoint(this);
            uVar6->calc();
        }
    }

    J3DJointCallBack jointCallback = getCallBack();
    if (jointCallback != NULL) {
        (*jointCallback)(this, 0);
    }

    J3DJoint* joint = getChild();
    if (joint != NULL) {
        joint->recursiveCalc();
    }
    MTXCopy(prevCurrentMtx, J3DSys::mCurrentMtx);

    J3DSys::mCurrentS = current;
    J3DSys::mParentS = parent;

    if (prevMtxCalc != NULL) {
        setCurrentMtxCalc(prevMtxCalc);
    }
    if (jointCallback != NULL) {
        (*jointCallback)(this, 1);
    }

    joint = getYounger();
    if (joint != NULL) {
        joint->recursiveCalc();
    }
}
