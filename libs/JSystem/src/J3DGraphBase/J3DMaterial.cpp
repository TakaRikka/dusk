#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DMaterial.h"
#include "JSystem/J3DGraphBase/J3DGD.h"

J3DColorBlock* J3DMaterial::createColorBlock(u32 flags) {
    J3DColorBlock* rv = NULL;
    switch (flags) {
    case 0:
        rv = new J3DColorBlockLightOff();
        break;
    case 0x40000000:
        rv = new J3DColorBlockLightOn();
        break;
    case 0x80000000:
        rv = new J3DColorBlockAmbientOn();
        break;
    }

    J3D_ASSERT_ALLOCMEM(55, rv != NULL);
    return rv;
}

J3DTexGenBlock* J3DMaterial::createTexGenBlock(u32 flags) {
    J3DTexGenBlock* rv = NULL;
    switch (flags) {
    case 0x8000000:
        rv = new J3DTexGenBlock4();
        break;
    case 0:
    default:
        rv = new J3DTexGenBlockBasic();
    }

    J3D_ASSERT_ALLOCMEM(83, rv != NULL);
    return rv;
}

J3DTevBlock* J3DMaterial::createTevBlock(int tevStageNum) {
    J3DTevBlock* rv = NULL;
    if (tevStageNum <= 1) {
        rv = new J3DTevBlock1();
    } else if (tevStageNum == 2) {
        rv = new J3DTevBlock2();
    } else if (tevStageNum <= 4) {
        rv = new J3DTevBlock4();
    } else if (tevStageNum <= 16) {
        rv = new J3DTevBlock16();
    }

    J3D_ASSERT_ALLOCMEM(116, rv != NULL);
    return rv;
}

J3DIndBlock* J3DMaterial::createIndBlock(int flags) {
    J3DIndBlock* rv = NULL;
    if (flags != 0) {
        rv = new J3DIndBlockFull();
    } else {
        rv = new J3DIndBlockNull();
    }

    J3D_ASSERT_ALLOCMEM(139, rv != NULL);
    return rv;
}

J3DPEBlock* J3DMaterial::createPEBlock(u32 flags, u32 materialMode) {
    J3DPEBlock* rv = NULL;
    if (flags == 0) {
        if (materialMode & 1) {
            rv = new J3DPEBlockOpa();
            J3D_ASSERT_ALLOCMEM(166, rv != NULL);
            return rv;
        } else if (materialMode & 2) {
            rv = new J3DPEBlockTexEdge();
            J3D_ASSERT_ALLOCMEM(172, rv != NULL);
            return rv;
        } else if (materialMode & 4) {
            rv = new J3DPEBlockXlu();
            J3D_ASSERT_ALLOCMEM(178, rv != NULL);
            return rv;
        }
    }

    if (flags == 0x10000000) {
        rv = new J3DPEBlockFull();
    } else if (flags == 0x20000000) {
        rv = new J3DPEBlockFogOff();
    }

    J3D_ASSERT_ALLOCMEM(188, rv != NULL);
    return rv;
}

u32 J3DMaterial::calcSizeColorBlock(u32 flags) {
    u32 rv = 0;
    switch (flags) {
    case 0:
        rv += sizeof(J3DColorBlockLightOff);
        break;
    case 0x40000000:
        rv += sizeof(J3DColorBlockLightOn);
        break;
    case 0x80000000:
        rv += sizeof(J3DColorBlockAmbientOn);
        break;
    }

    return rv;
}

u32 J3DMaterial::calcSizeTexGenBlock(u32 flags) {
    u32 rv = 0;
    switch (flags) {
    case 0x8000000:
        rv += sizeof(J3DTexGenBlock4);
        break;
    case 0:
    default:
        rv += sizeof(J3DTexGenBlockBasic);
    }

    return rv;
}

u32 J3DMaterial::calcSizeTevBlock(int tevStageNum) {
    u32 rv = 0;
    if (tevStageNum <= 1) {
        rv += sizeof(J3DTevBlock1);
    } else if (tevStageNum == 2) {
        rv += sizeof(J3DTevBlock2);
    } else if (tevStageNum <= 4) {
        rv += sizeof(J3DTevBlock4);
    } else if (tevStageNum <= 16) {
        rv += sizeof(J3DTevBlock16);
    }

    return rv;
}

u32 J3DMaterial::calcSizeIndBlock(int flags) {
    u32 rv = 0;
    if (flags != 0) {
        rv += sizeof(J3DIndBlockFull);
    } else {
        rv += sizeof(J3DIndBlockNull);
    }

    return rv;
}

u32 J3DMaterial::calcSizePEBlock(u32 flags, u32 materialMode) {
    u32 rv = 0;
    if (flags == 0) {
        if (materialMode & 1) {
            rv += sizeof(J3DPEBlockOpa);
        } else if (materialMode & 2) {
            rv += sizeof(J3DPEBlockTexEdge);
        } else if (materialMode & 4) {
            rv += sizeof(J3DPEBlockXlu);
        }
    } else if (flags == 0x10000000) {
        rv += sizeof(J3DPEBlockFull);
    } else if (flags == 0x20000000) {
        rv += sizeof(J3DPEBlockFogOff);
    }

    return rv;
}

void J3DMaterial::initialize() {
    mShape = NULL;
    mNext = NULL;
    mJoint = NULL;
    mMaterialMode = 1;
    mIndex = -1;
    mInvalid = 0;
    mDiffFlag = 0;
    mColorBlock = NULL;
    mTexGenBlock = NULL;
    mTevBlock = NULL;
    mIndBlock = NULL;
    mPEBlock = NULL;
    mpOrigMaterial = NULL;
    mMaterialAnm = NULL;
    mSharedDLObj = NULL;
}

u32 J3DMaterial::countDLSize() {
    return (mColorBlock->countDLSize() + mTexGenBlock->countDLSize() + mTevBlock->countDLSize() +
        mIndBlock->countDLSize() + mPEBlock->countDLSize() + 31) & ~0x1f;
}

void J3DMaterial::makeDisplayList_private(J3DDisplayListObj* pDLObj) {
    pDLObj->beginDL();
    mTevBlock->load();
    mIndBlock->load();
    mPEBlock->load();
    J3DGDSetGenMode(mTexGenBlock->getTexGenNum(), mColorBlock->getColorChanNum(), mTevBlock->getTevStageNum(), mIndBlock->getIndTexStageNum(), (GXCullMode)(u8)mColorBlock->getCullMode());
    mTexGenBlock->load();
    mColorBlock->load();
    J3DGDSetNumChans(mColorBlock->getColorChanNum());
    J3DGDSetNumTexGens(mTexGenBlock->getTexGenNum());
    pDLObj->endDL();
}

void J3DMaterial::makeDisplayList() {
    if (!j3dSys.getMatPacket()->isLocked()) {
        j3dSys.getMatPacket()->mDiffFlag = mDiffFlag;
        makeDisplayList_private(j3dSys.getMatPacket()->getDisplayListObj());
    }
}

void J3DMaterial::makeSharedDisplayList() {
    makeDisplayList_private(mSharedDLObj);
}

void J3DMaterial::load() {
    j3dSys.setMaterialMode(mMaterialMode);
    if (!j3dSys.checkFlag(2)) {
        loadNBTScale(*mTexGenBlock->getNBTScale());
    }
}

void J3DMaterial::loadSharedDL() {
    j3dSys.setMaterialMode(mMaterialMode);
    if (!j3dSys.checkFlag(2)) {
        mSharedDLObj->callDL();
        loadNBTScale(*mTexGenBlock->getNBTScale());
    }
}

void J3DMaterial::patch() {
    j3dSys.getMatPacket()->mDiffFlag = mDiffFlag;
    j3dSys.getMatPacket()->beginPatch();
    mTevBlock->patch();
    mColorBlock->patch();
    mTexGenBlock->patch();
    j3dSys.getMatPacket()->endPatch();
}

void J3DMaterial::diff(u32 diffFlags) {
    if (j3dSys.getMatPacket()->isEnabled_Diff()) {
        j3dSys.getMatPacket()->beginDiff();

        mTevBlock->diff(diffFlags);
        mIndBlock->diff(diffFlags);
        mPEBlock->diff(diffFlags);
        if (diffFlags & J3DDiffFlag_KonstColor) {
            J3DGDSetGenMode_3Param(mTexGenBlock->getTexGenNum(), mTevBlock->getTevStageNum(), mIndBlock->getIndTexStageNum());
            J3DGDSetNumTexGens(mTexGenBlock->getTexGenNum());
        }
        mTexGenBlock->diff(diffFlags);
        mColorBlock->diff(diffFlags);

        j3dSys.getMatPacket()->endDiff();
    }
}

void J3DMaterial::calc(f32 const (*param_0)[4]) {
    if (j3dSys.checkFlag(0x40000000)) {
        mTexGenBlock->calcPostTexMtx(param_0);
    } else {
        mTexGenBlock->calc(param_0);
    }

    calcCurrentMtx();
    setCurrentMtx();
}

void J3DMaterial::calcDiffTexMtx(f32 const (*param_0)[4]) {
    if (j3dSys.checkFlag(0x40000000)) {
        mTexGenBlock->calcPostTexMtxWithoutViewMtx(param_0);
    } else {
        mTexGenBlock->calcWithoutViewMtx(param_0);
    }
}

void J3DMaterial::setCurrentMtx() {
    mShape->setCurrentMtx(mCurrentMtx);
}

void J3DMaterial::calcCurrentMtx() {
    if (!j3dSys.checkFlag(0x40000000)) {
        mCurrentMtx.setCurrentTexMtx(
            getTexCoord(0)->getTexGenMtx(),
            getTexCoord(1)->getTexGenMtx(),
            getTexCoord(2)->getTexGenMtx(),
            getTexCoord(3)->getTexGenMtx(),
            getTexCoord(4)->getTexGenMtx(),
            getTexCoord(5)->getTexGenMtx(),
            getTexCoord(6)->getTexGenMtx(),
            getTexCoord(7)->getTexGenMtx()
        );
    } else {
        mCurrentMtx.setCurrentTexMtx(
            getTexCoord(0)->getTexMtxReg(),
            getTexCoord(1)->getTexMtxReg(),
            getTexCoord(2)->getTexMtxReg(),
            getTexCoord(3)->getTexMtxReg(),
            getTexCoord(4)->getTexMtxReg(),
            getTexCoord(5)->getTexMtxReg(),
            getTexCoord(6)->getTexMtxReg(),
            getTexCoord(7)->getTexMtxReg()
        );
    }
}

void J3DMaterial::copy(J3DMaterial* pOther) {
    J3D_ASSERT_NULLPTR(620, pOther != NULL);
    mColorBlock->reset(pOther->mColorBlock);
    mTexGenBlock->reset(pOther->mTexGenBlock);
    mTevBlock->reset(pOther->mTevBlock);
    mIndBlock->reset(pOther->mIndBlock);
    mPEBlock->reset(pOther->mPEBlock);
}

void J3DMaterial::reset() {
    if ((~mDiffFlag & J3DDiffFlag_Changed) == 0) {
        mDiffFlag &= ~J3DDiffFlag_Changed;
        mMaterialMode = mpOrigMaterial->mMaterialMode;
        mInvalid = mpOrigMaterial->mInvalid;
        mMaterialAnm = NULL;
        copy(mpOrigMaterial);
    }
}

void J3DMaterial::change() {
    if ((mDiffFlag & (J3DDiffFlag_Changed | J3DDiffFlag_Unk40000000)) == 0) {
        mDiffFlag |= J3DDiffFlag_Changed;
        mMaterialAnm = NULL;
    }
}

s32 J3DMaterial::newSharedDisplayList(u32 dlSize) {
    if (mSharedDLObj == NULL) {
        mSharedDLObj = new J3DDisplayListObj();
        if (mSharedDLObj == NULL) {
            return kJ3DError_Alloc;
        }

        s32 ret = mSharedDLObj->newDisplayList(dlSize);
        if (ret != kJ3DError_Success) {
            return ret;
        }
    }

    return kJ3DError_Success;
}

s32 J3DMaterial::newSingleSharedDisplayList(u32 dlSize) {
    if (mSharedDLObj == NULL) {
        mSharedDLObj = new J3DDisplayListObj();
        if (mSharedDLObj == NULL) {
            return kJ3DError_Alloc;
        }

        s32 ret = mSharedDLObj->newSingleDisplayList(dlSize);
        if (ret != kJ3DError_Success) {
            return ret;
        }
    }

    return kJ3DError_Success;
}

void J3DPatchedMaterial::initialize() {
    J3DMaterial::initialize();
}

void J3DPatchedMaterial::makeDisplayList() {}

void J3DPatchedMaterial::makeSharedDisplayList() {}

void J3DPatchedMaterial::load() {
    j3dSys.setMaterialMode(mMaterialMode);
    if (j3dSys.checkFlag(2)) {
        return;
    }
}

void J3DPatchedMaterial::loadSharedDL() {
    j3dSys.setMaterialMode(mMaterialMode);
    if (!j3dSys.checkFlag(0x02))
        mSharedDLObj->callDL();
}

void J3DPatchedMaterial::reset() {}

void J3DPatchedMaterial::change() {}

void J3DLockedMaterial::initialize() {
    J3DMaterial::initialize();
}

void J3DLockedMaterial::makeDisplayList() {}

void J3DLockedMaterial::makeSharedDisplayList() {}

void J3DLockedMaterial::load() {
    j3dSys.setMaterialMode(mMaterialMode);
    if (j3dSys.checkFlag(2)) {
        return;
    }
}

void J3DLockedMaterial::loadSharedDL() {
    j3dSys.setMaterialMode(mMaterialMode);
    if (!j3dSys.checkFlag(0x02))
        mSharedDLObj->callDL();
}

void J3DLockedMaterial::patch() {}

void J3DLockedMaterial::diff(u32 diffFlags) {}

void J3DLockedMaterial::calc(const Mtx param_0) {}

void J3DLockedMaterial::reset() {}

void J3DLockedMaterial::change() {}
