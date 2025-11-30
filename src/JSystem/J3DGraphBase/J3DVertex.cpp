#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphBase/J3DVertex.h"
#include "JSystem/J3DGraphAnimator/J3DJointTree.h"
#include "JSystem/J3DGraphBase/J3DSys.h"
#include "JSystem/JKernel/JKRHeap.h"
#include <string.h>

J3DVertexData::J3DVertexData() {
    mVtxNum = 0;
    mNrmNum = 0;
    mColNum = 0;
    mTexCoordNum = 0;
    mPacketNum = 0;

    mVtxAttrFmtList = NULL;
    mVtxPosArray = NULL;
    mVtxNrmArray = NULL;
    mVtxNBTArray = NULL;

    for (int i = 0; i < 2; i++)
        mVtxColorArray[i] = NULL;

    for (int i = 0; i < 8; i++)
        mVtxTexCoordArray[i] = NULL;

    mVtxPosFrac = 0;
    mVtxPosType = GX_F32;
    mVtxNrmFrac = 0;
    mVtxNrmType = GX_F32;
}

void J3DVertexBuffer::setVertexData(J3DVertexData* pVtxData) {
    J3D_ASSERT_NULLPTR(175, pVtxData != NULL);

    mVtxData = pVtxData;
    mVtxPosArray[0] = pVtxData->getVtxPosArray();
    mVtxNrmArray[0] = pVtxData->getVtxNrmArray();
    mVtxColArray[0] = pVtxData->getVtxColorArray(0);
    mVtxPosArray[1] = NULL;
    mVtxNrmArray[1] = NULL;
    mVtxColArray[1] = NULL;

    mTransformedVtxPosArray[0] = pVtxData->getVtxPosArray();
    mTransformedVtxNrmArray[0] = pVtxData->getVtxNrmArray();
    mTransformedVtxPosArray[1] = NULL;
    mTransformedVtxNrmArray[1] = NULL;

    frameInit();
}

void J3DVertexBuffer::init() {
    mVtxData = NULL;

    mVtxPosArray[0] = mVtxPosArray[1] = NULL;
    mVtxNrmArray[0] = mVtxNrmArray[1] = NULL;
    mVtxColArray[0] = mVtxColArray[1] = NULL;
    mTransformedVtxPosArray[0] = mTransformedVtxPosArray[1] = NULL;
    mTransformedVtxNrmArray[0] = mTransformedVtxNrmArray[1] = NULL;

    mCurrentVtxPos = NULL;
    mCurrentVtxNrm = NULL;
    mCurrentVtxCol = NULL;

    frameInit();
}

J3DVertexBuffer::~J3DVertexBuffer() {}

void J3DVertexBuffer::setArray() const {
    j3dSys.setVtxPos(mCurrentVtxPos);
    j3dSys.setVtxNrm(mCurrentVtxNrm);
    j3dSys.setVtxCol(mCurrentVtxCol);
}

s32 J3DVertexBuffer::copyLocalVtxPosArray(u32 flag) {
    if (flag & 1) {
        for (int i = 0; i < 2; i++) {
            mVtxPosArray[i] = new (0x20) char[mVtxData->getVtxNum() * 3 * 4];
            if (mVtxPosArray[i] == NULL) {
                return kJ3DError_Alloc;
            }

            memcpy(mVtxPosArray[i], mVtxData->getVtxPosArray(), mVtxData->getVtxNum() * 3 * 4);
            DCStoreRange(mVtxPosArray[i], mVtxData->getVtxNum() * 3 * 4);
        }
    } else {
        mVtxPosArray[0] = mVtxData->getVtxPosArray();

        if (mVtxPosArray[1] == NULL) {
            mVtxPosArray[1] = new (0x20) char[mVtxData->getVtxNum() * 3 * 4];
            if (mVtxPosArray[1] == NULL) {
                return kJ3DError_Alloc;
            }
        }

        memcpy(mVtxPosArray[1], mVtxData->getVtxPosArray(), mVtxData->getVtxNum() * 3 * 4);
        DCStoreRange(mVtxPosArray[1], mVtxData->getVtxNum() * 3 * 4);
    }

    return kJ3DError_Success;
}

s32 J3DVertexBuffer::copyLocalVtxNrmArray(u32 flag) {
    if (flag & 1) {
        for (int i = 0; i < 2; i++) {
            mVtxNrmArray[i] = new (0x20) char[mVtxData->getNrmNum() * 3 * 4];
            if (mVtxNrmArray[i] == NULL) {
                return kJ3DError_Alloc;
            }

            memcpy(mVtxNrmArray[i], mVtxData->getVtxNrmArray(), mVtxData->getNrmNum() * 3 * 4);
            DCStoreRange(mVtxNrmArray[i], mVtxData->getNrmNum() * 3 * 4);
        }
    } else {
        mVtxNrmArray[0] = mVtxData->getVtxNrmArray();

        if (mVtxNrmArray[1] == NULL) {
            mVtxNrmArray[1] = new (0x20) char[mVtxData->getNrmNum() * 3 * 4];
            if (mVtxNrmArray[1] == NULL) {
                return kJ3DError_Alloc;
            }
        }

        memcpy(mVtxNrmArray[1], mVtxData->getVtxNrmArray(), mVtxData->getNrmNum() * 3 * 4);
        DCStoreRange(mVtxNrmArray[1], mVtxData->getNrmNum() * 3 * 4);
    }

    return kJ3DError_Success;
}

s32 J3DVertexBuffer::copyLocalVtxArray(u32 flag) {
    void* oldPosArray[2];
    void* oldNrmArray[2];

    for (int i = 0; i < 2; i++)
        oldPosArray[i] = mVtxPosArray[i];

    if (~flag & 2) {
        s32 ret = copyLocalVtxPosArray(flag);
        if (ret != 0) {
            for (int i = 0; i < 2; i++) {
                if (oldPosArray[i] != mVtxPosArray[i]) {
                    if (mVtxPosArray[i] != mVtxData->getVtxPosArray())
                        delete mVtxPosArray[i];
                    mVtxPosArray[i] = oldPosArray[i];
                }
            }
            return ret;
        }
    } else {
        mVtxPosArray[0] = mVtxPosArray[1] = mVtxData->getVtxPosArray();
    }

    for (int i = 0; i < 2; i++)
        oldNrmArray[i] = mVtxNrmArray[i];

    if (~flag & 4) {
        s32 ret = copyLocalVtxNrmArray(flag);
        if (ret != 0) {
            for (int i = 0; i < 2; i++) {
                if (oldPosArray[i] != mVtxPosArray[i]) {
                    if (mVtxPosArray[i] != mVtxData->getVtxPosArray())
                        delete mVtxPosArray[i];
                    mVtxPosArray[i] = oldPosArray[i];
                }

                if (oldNrmArray[i] != mVtxNrmArray[i]) {
                    if (mVtxNrmArray[i] != mVtxData->getVtxNrmArray())
                        delete mVtxNrmArray[i];
                    mVtxNrmArray[i] = oldNrmArray[i];
                }
            }
            return ret;
        }
    } else {
        mVtxNrmArray[0] = mVtxNrmArray[1] = mVtxData->getVtxNrmArray();
    }

    return kJ3DError_Success;
}

s32 J3DVertexBuffer::allocTransformedVtxPosArray() {
    if (mTransformedVtxPosArray[0] != NULL && mTransformedVtxPosArray[1] != NULL)
        return kJ3DError_Success;

    for (int i = 0; i < 2; i++) {
        if (i == 0 || mTransformedVtxPosArray[i] == NULL) {
            mTransformedVtxPosArray[i] = new (0x20) char[mVtxData->getVtxNum() * 3 * 4];
            if (mTransformedVtxPosArray[i] == NULL)
                return kJ3DError_Alloc;
        }
    }

    return kJ3DError_Success;
}

s32 J3DVertexBuffer::allocTransformedVtxNrmArray() {
    if (mTransformedVtxNrmArray[0] != NULL && mTransformedVtxNrmArray[1] != NULL)
        return kJ3DError_Success;

    for (int i = 0; i < 2; i++) {
        if (i == 0 || mTransformedVtxNrmArray[i] == NULL) {
            mTransformedVtxNrmArray[i] = new (0x20) char[mVtxData->getNrmNum() * 3 * 4];
            if (mTransformedVtxNrmArray[i] == NULL)
                return kJ3DError_Alloc;
        }
    }

    return kJ3DError_Success;
}

J3DDrawMtxData::J3DDrawMtxData() {
    mEntryNum = 0;
    mDrawMtxFlag = NULL;
    mDrawMtxIndex = NULL;
}

J3DDrawMtxData::~J3DDrawMtxData() {}
