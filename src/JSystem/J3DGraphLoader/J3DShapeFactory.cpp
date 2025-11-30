#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphLoader/J3DShapeFactory.h"
#include "JSystem/J3DGraphBase/J3DShape.h"
#include "JSystem/J3DGraphBase/J3DShapeMtx.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport/JSupport.h"
#include "dolphin/os.h"
#include "global.h"

J3DShapeFactory::J3DShapeFactory(J3DShapeBlock const& block) {
    mShapeInitData = JSUConvertOffsetToPtr<J3DShapeInitData>(&block, (uintptr_t)block.mpShapeInitData);
    mIndexTable = JSUConvertOffsetToPtr<u16>(&block, (uintptr_t)block.mpIndexTable);
    mVtxDescList = JSUConvertOffsetToPtr<GXVtxDescList>(&block, (uintptr_t)block.mpVtxDescList),
    mMtxTable = JSUConvertOffsetToPtr<u16>(&block, (uintptr_t)block.mpMtxTable);
    mDisplayListData = JSUConvertOffsetToPtr<u8>(&block, (uintptr_t)block.mpDisplayListData),
    mMtxInitData = JSUConvertOffsetToPtr<J3DShapeMtxInitData>(&block, (uintptr_t)block.mpMtxInitData),
    mDrawInitData = JSUConvertOffsetToPtr<J3DShapeDrawInitData>(&block, (uintptr_t)block.mpDrawInitData),
    mVcdVatCmdBuffer = NULL;
}

J3DShape* J3DShapeFactory::create(int no, u32 flag, GXVtxDescList* vtxDesc) {
    J3DShape* shape = new J3DShape;
    shape->mMtxGroupNum = getMtxGroupNum(no);
    shape->mRadius = getRadius(no);
    shape->mVtxDesc = getVtxDescList(no);
    shape->mShapeMtx = new J3DShapeMtx*[shape->mMtxGroupNum];
    shape->mShapeDraw = new J3DShapeDraw*[shape->mMtxGroupNum];
    shape->mMin = getMin(no);
    shape->mMax = getMax(no);
    shape->mVcdVatCmd = mVcdVatCmdBuffer + no * J3DShape::kVcdVatDLSize;

    for (s32 i = 0; i < shape->mMtxGroupNum; i++) {
        shape->mShapeMtx[i] = newShapeMtx(flag, no, i);
        shape->mShapeDraw[i] = newShapeDraw(no, i);
    }

    shape->mIndex = no;
    return shape;
}

enum {
    J3DMdlDataFlag_ConcatView = 0x10,
};

enum {
    J3DShapeMtxType_Mtx = 0x00,
    J3DShapeMtxType_BBoard = 0x01,
    J3DShapeMtxType_YBBoard = 0x02,
    J3DShapeMtxType_Multi = 0x03,
};

J3DShapeMtx* J3DShapeFactory::newShapeMtx(u32 flag, int shapeNo, int mtxGroupNo) const {
    J3DShapeMtx* ret = NULL;
    const J3DShapeInitData& shapeInitData = mShapeInitData[mIndexTable[shapeNo]];
    const J3DShapeMtxInitData& mtxInitData =
        (&mMtxInitData[shapeInitData.mMtxInitDataIndex])[mtxGroupNo];

    switch (getMdlDataFlag_MtxLoadType(flag)) {
    case J3DMdlDataFlag_ConcatView:
        switch (shapeInitData.mShapeMtxType) {
        case J3DShapeMtxType_Mtx:
            ret = new J3DShapeMtxConcatView(mtxInitData.mUseMtxIndex);
            break;
        case J3DShapeMtxType_BBoard:
            ret = new J3DShapeMtxBBoardConcatView(mtxInitData.mUseMtxIndex);
            break;
        case J3DShapeMtxType_YBBoard:
            ret = new J3DShapeMtxYBBoardConcatView(mtxInitData.mUseMtxIndex);
            break;
        case J3DShapeMtxType_Multi:
            ret = new J3DShapeMtxMultiConcatView(mtxInitData.mUseMtxIndex, mtxInitData.mUseMtxCount,
                                                 &mMtxTable[mtxInitData.mFirstUseMtxIndex]);
            break;
        default:
            OSReport("WRONG SHAPE MATRIX TYPE (J3DModelInit.cpp)\n");
            break;
        }
        break;

    case 0:
    default:
        switch (shapeInitData.mShapeMtxType) {
        case J3DShapeMtxType_Mtx:
        case J3DShapeMtxType_BBoard:
        case J3DShapeMtxType_YBBoard:
            ret = new J3DShapeMtx(mtxInitData.mUseMtxIndex);
            break;
        case J3DShapeMtxType_Multi:
            ret = new J3DShapeMtxMulti(mtxInitData.mUseMtxIndex, mtxInitData.mUseMtxCount,
                                       &mMtxTable[mtxInitData.mFirstUseMtxIndex]);
            break;
        default:
            OSReport("WRONG SHAPE MATRIX TYPE (J3DModelInit.cpp)\n");
            break;
        }
        break;
    }

    return ret;
}

J3DShapeDraw* J3DShapeFactory::newShapeDraw(int shapeNo, int mtxGroupNo) const {
    const J3DShapeInitData& shapeInitData = mShapeInitData[mIndexTable[shapeNo]];
    const J3DShapeDrawInitData& drawInitData =
        (&mDrawInitData[shapeInitData.mDrawInitDataIndex])[mtxGroupNo];
    return new J3DShapeDraw(&mDisplayListData[drawInitData.mDisplayListIndex],
                            drawInitData.mDisplayListSize);
}

void J3DShapeFactory::allocVcdVatCmdBuffer(u32 count) {
    mVcdVatCmdBuffer = new (0x20) u8[J3DShape::kVcdVatDLSize * count];
    for (u32 i = 0; i < (J3DShape::kVcdVatDLSize * count) / 4; i++)
        ((u32*)mVcdVatCmdBuffer)[i] = 0;
}

s32 J3DShapeFactory::calcSize(int shapeNo, u32 flag) {
    s32 size = 0x68;

    s32 mtxGroupNo = getMtxGroupNum(shapeNo);
    size += mtxGroupNo * 4;
    size += mtxGroupNo * 4;

    for (u32 i = 0; i < mtxGroupNo; i++) {
        s32 shapeMtxSize = calcSizeShapeMtx(flag, shapeNo, i);
        size += shapeMtxSize;
        size += 0x0C;
    }

    return size;
}

s32 J3DShapeFactory::calcSizeVcdVatCmdBuffer(u32 count) {
    return ALIGN_NEXT(count * J3DShape::kVcdVatDLSize, 0x20);
}

s32 J3DShapeFactory::calcSizeShapeMtx(u32 flag, int shapeNo, int mtxGroupNo) const {
    const J3DShapeInitData& shapeInitData = mShapeInitData[mIndexTable[shapeNo]];
    u32 ret = 0;

    u32 mtxLoadType = getMdlDataFlag_MtxLoadType(flag);
    switch (mtxLoadType) {
    case J3DMdlDataFlag_ConcatView:
        switch (shapeInitData.mShapeMtxType) {
        case J3DShapeMtxType_Mtx:
            ret = 0x08;
            break;
        case J3DShapeMtxType_BBoard:
            ret = 0x08;
            break;
        case J3DShapeMtxType_YBBoard:
            ret = 0x08;
            break;
        case J3DShapeMtxType_Multi:
            ret = 0x10;
            break;
        default:
            OSReport("WRONG SHAPE MATRIX TYPE (J3DModelInit.cpp)\n");
        }
        break;

    case 0:
    default:
        switch (shapeInitData.mShapeMtxType) {
        case J3DShapeMtxType_Mtx:
        case J3DShapeMtxType_BBoard:
        case J3DShapeMtxType_YBBoard:
            ret = 0x08;
            break;
        case J3DShapeMtxType_Multi:
            ret = 0x10;
            break;
        default:
            OSReport("WRONG SHAPE MATRIX TYPE (J3DModelInit.cpp)\n");
        }
        break;
    }

    return ret;
}
