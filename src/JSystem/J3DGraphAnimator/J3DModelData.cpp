//
// J3DModelData
//

#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphAnimator/J3DModelData.h"
#include "JSystem/J3DGraphAnimator/J3DMaterialAnm.h"
#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/J3DGraphAnimator/J3DModel.h"

void J3DModelData::clear() {
    mpRawData = 0;
    mFlags = 0;
    mbHasBumpArray = 0;
    mbHasBillboard = 0;
}

J3DModelData::J3DModelData() {
    clear();
}

s32 J3DModelData::newSharedDisplayList(u32 mdlFlags) {
    u16 matNum = getMaterialNum();

    for (u16 i = 0; i < matNum; i++) {
        s32 ret;
        if (mdlFlags & J3DMdlFlag_UseSingleDL) {
            ret = getMaterialNodePointer(i)->newSingleSharedDisplayList(getMaterialNodePointer(i)->countDLSize());
            if (ret != kJ3DError_Success)
                return ret;
        } else {
            ret = getMaterialNodePointer(i)->newSharedDisplayList(getMaterialNodePointer(i)->countDLSize());
            if (ret != kJ3DError_Success)
                return ret;
        }
    }

    return kJ3DError_Success;
}

void J3DModelData::indexToPtr() {
    j3dSys.setTexture(getTexture());

    static BOOL sInterruptFlag = OSDisableInterrupts();
    OSDisableScheduler();

    GDLObj gdl_obj;
    u16 matNum = getMaterialNum();
    for (u16 i = 0; i < matNum; i++) {
        J3DMaterial* matNode = getMaterialNodePointer(i);
        J3DDisplayListObj* dl_obj = matNode->getSharedDisplayListObj();

        GDInitGDLObj(&gdl_obj, dl_obj->getDisplayList(0), dl_obj->getDisplayListSize());
        GDSetCurrent(&gdl_obj);
        matNode->getTevBlock()->indexToPtr();
    }

    GDSetCurrent(NULL);
    OSEnableScheduler();
    OSRestoreInterrupts(sInterruptFlag);
}

void J3DModelData::makeSharedDL() {
    j3dSys.setTexture(getTexture());

    u16 matNum = getMaterialNum();
    for (u16 i = 0; i < matNum; i++) {
        getMaterialNodePointer(i)->makeSharedDisplayList();
    }
}

void J3DModelData::simpleCalcMaterial(u16 idx, Mtx param_1) {
    syncJ3DSysFlags();

    J3DMaterial* mat;
    J3DJoint* jointNode = getJointNodePointer(idx);
    for (mat = jointNode->getMesh(); mat != NULL; mat = mat->getNext()) {
        if (mat->getMaterialAnm() != NULL) {
            mat->getMaterialAnm()->calc(mat);
        }
        mat->calc(param_1);
    }
}

void J3DModelData::syncJ3DSysPointers() const {
    j3dSys.setTexture(getTexture());
    j3dSys.setVtxPos(getVtxPosArray());
    j3dSys.setVtxNrm(getVtxNrmArray());
    j3dSys.setVtxCol(getVtxColorArray(0));
}

void J3DModelData::syncJ3DSysFlags() const {
    if (checkFlag(0x20)) {
        j3dSys.onFlag(J3DSysFlag_PostTexMtx);
    } else {
        j3dSys.offFlag(J3DSysFlag_PostTexMtx);
    }
}
