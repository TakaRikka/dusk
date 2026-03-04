#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphLoader/J3DShapeFactory.h"
#include "JSystem/J3DGraphBase/J3DShape.h"
#include "JSystem/J3DGraphBase/J3DShapeMtx.h"
#include "JSystem/JKernel/JKRHeap.h"
#include "JSystem/JSupport/JSupport.h"
#include <os.h>
#include "global.h"
#include <algorithm>

J3DShapeFactory::J3DShapeFactory(J3DShapeBlock const& block) {
    mShapeInitData = JSUConvertOffsetToPtr<J3DShapeInitData>(&block, (uintptr_t)block.mpShapeInitData);
    mIndexTable = JSUConvertOffsetToPtr<BE(u16)>(&block, (uintptr_t)block.mpIndexTable);
    mVtxDescList = JSUConvertOffsetToPtr<GXVtxDescList>(&block, (uintptr_t)block.mpVtxDescList);
    mMtxTable = JSUConvertOffsetToPtr<BE(u16)>(&block, (uintptr_t)block.mpMtxTable);
    mDisplayListData = JSUConvertOffsetToPtr<u8>(&block, (uintptr_t)block.mpDisplayListData);
    mMtxInitData = JSUConvertOffsetToPtr<J3DShapeMtxInitData>(&block, (uintptr_t)block.mpMtxInitData);
    mDrawInitData = JSUConvertOffsetToPtr<J3DShapeDrawInitData>(&block, (uintptr_t)block.mpDrawInitData);
    mVcdVatCmdBuffer = NULL;

#if TARGET_LITTLE_ENDIAN
    // mVtxDescList is in big endian, swap to little endian.
    int maxVtxDescListStart = 0;
    for (int shapeIdx = 0; shapeIdx < block.mShapeNum; shapeIdx++) {
        u16 thisIndex = mShapeInitData[mIndexTable[shapeIdx]].mVtxDescListIndex;
        maxVtxDescListStart = std::max(
            maxVtxDescListStart,
            (int)(thisIndex / sizeof(GXVtxDescList)));
    }

    GXVtxDescList* lastEntry = mVtxDescList + maxVtxDescListStart;
    while (lastEntry->attr != BE(GXAttr)::swap(GX_VA_NULL)) {
        lastEntry++;
    }

    for (GXVtxDescList* entry = mVtxDescList; entry <= lastEntry; entry++) {
        *entry = BE(GXVtxDescList)::swap(*entry);
    }
#endif
}

J3DShape* J3DShapeFactory::create(int no, u32 flag, GXVtxDescList* vtxDesc) {
    J3DShape* shape = JKR_NEW J3DShape;
    J3D_ASSERT_ALLOCMEM(67, shape);
    shape->mMtxGroupNum = getMtxGroupNum(no);
    shape->mRadius = getRadius(no);
    shape->mVtxDesc = getVtxDescList(no);
    shape->mShapeMtx = JKR_NEW J3DShapeMtx*[shape->mMtxGroupNum];
    J3D_ASSERT_ALLOCMEM(74, shape->mShapeMtx);
    shape->mShapeDraw = JKR_NEW J3DShapeDraw*[shape->mMtxGroupNum];
    J3D_ASSERT_ALLOCMEM(76, shape->mShapeDraw);
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

    u32 mtxLoadType = getMdlDataFlag_MtxLoadType(flag);
    switch (mtxLoadType) {
    case J3DMdlDataFlag_ConcatView:
        switch (shapeInitData.mShapeMtxType) {
        case J3DShapeMtxType_Mtx:
            ret = JKR_NEW J3DShapeMtxConcatView(mtxInitData.mUseMtxIndex);
            break;
        case J3DShapeMtxType_BBoard:
            ret = JKR_NEW J3DShapeMtxBBoardConcatView(mtxInitData.mUseMtxIndex);
            break;
        case J3DShapeMtxType_YBBoard:
            ret = JKR_NEW J3DShapeMtxYBBoardConcatView(mtxInitData.mUseMtxIndex);
            break;
        case J3DShapeMtxType_Multi:
            ret = JKR_NEW J3DShapeMtxMultiConcatView(mtxInitData.mUseMtxIndex, mtxInitData.mUseMtxCount,
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
            ret = JKR_NEW J3DShapeMtx(mtxInitData.mUseMtxIndex);
            break;
        case J3DShapeMtxType_Multi:
            ret = JKR_NEW J3DShapeMtxMulti(mtxInitData.mUseMtxIndex, mtxInitData.mUseMtxCount,
                                       &mMtxTable[mtxInitData.mFirstUseMtxIndex]);
            break;
        default:
            OSReport("WRONG SHAPE MATRIX TYPE (J3DModelInit.cpp)\n");
            break;
        }

        break;
    }

    J3D_ASSERT_ALLOCMEM(167, ret);
    return ret;
}

J3DShapeDraw* J3DShapeFactory::newShapeDraw(int shapeNo, int mtxGroupNo) const {
    J3DShapeDraw* shapeDraw = NULL;
    const J3DShapeInitData& shapeInitData = mShapeInitData[mIndexTable[shapeNo]];
    const J3DShapeDrawInitData& drawInitData =
        (&mDrawInitData[shapeInitData.mDrawInitDataIndex])[mtxGroupNo];
    shapeDraw = JKR_NEW J3DShapeDraw(&mDisplayListData[drawInitData.mDisplayListIndex], drawInitData.mDisplayListSize);
    J3D_ASSERT_ALLOCMEM(193, shapeDraw);
    return shapeDraw;
}

void J3DShapeFactory::allocVcdVatCmdBuffer(u32 count) {
    mVcdVatCmdBuffer = JKR_NEW_ARGS (0x20) u8[J3DShape::kVcdVatDLSize * count];
    J3D_ASSERT_ALLOCMEM(211, mVcdVatCmdBuffer);
    for (u32 i = 0; i < (J3DShape::kVcdVatDLSize * count) / 4; i++)
        ((u32*)mVcdVatCmdBuffer)[i] = 0;
}

s32 J3DShapeFactory::calcSize(int shapeNo, u32 flag) {
    s32 size = 0;

    s32 mtxGroupNo = getMtxGroupNum(shapeNo);
    size += sizeof(J3DShape);
    size += mtxGroupNo * 4;
    size += mtxGroupNo * 4;

    for (u32 i = 0; i < mtxGroupNo; i++) {
        size += calcSizeShapeMtx(flag, shapeNo, i);
        size += 0x0C;
    }

    return size;
}

s32 J3DShapeFactory::calcSizeVcdVatCmdBuffer(u32 count) {
    return ALIGN_NEXT(count * J3DShape::kVcdVatDLSize, 0x20);
}

s32 J3DShapeFactory::calcSizeShapeMtx(u32 flag, int shapeNo, int mtxGroupNo) const {
    int local_18 = 0;
    const J3DShapeInitData& shapeInitData = mShapeInitData[mIndexTable[shapeNo]];
    J3DShapeMtxInitData& mtxInitData = (&mMtxInitData[shapeInitData.mMtxInitDataIndex])[mtxGroupNo];
    u32 ret = 0;

    u32 mtxLoadType = getMdlDataFlag_MtxLoadType(flag);
    switch (mtxLoadType) {
    case J3DMdlDataFlag_ConcatView:
        switch (shapeInitData.mShapeMtxType) {
        case J3DShapeMtxType_Mtx:
            ret += sizeof(J3DShapeMtxConcatView);
            break;
        case J3DShapeMtxType_BBoard:
            ret += sizeof(J3DShapeMtxBBoardConcatView);
            break;
        case J3DShapeMtxType_YBBoard:
            ret += sizeof(J3DShapeMtxYBBoardConcatView);
            break;
        case J3DShapeMtxType_Multi:
            ret += sizeof(J3DShapeMtxMultiConcatView);
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
            ret += 0x08;
            break;
        case J3DShapeMtxType_Multi:
            ret += sizeof(J3DShapeMtxMultiConcatView);
            break;
        default:
            OSReport("WRONG SHAPE MATRIX TYPE (J3DModelInit.cpp)\n");
        }
        break;
    }

    return ret;
}
