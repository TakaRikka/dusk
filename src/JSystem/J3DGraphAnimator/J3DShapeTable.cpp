#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/J3DGraphAnimator/J3DShapeTable.h"
#include "JSystem/J3DGraphBase/J3DShape.h"

void J3DShapeTable::hide() {
    u16 shapeNum = mShapeNum;
    for (u16 i = 0; i < shapeNum; i++) {
        mShapeNodePointer[i]->onFlag(1);
    }
}

void J3DShapeTable::show() {
    u16 shapeNum = mShapeNum;
    for (u16 i = 0; i < shapeNum; i++) {
        mShapeNodePointer[i]->offFlag(1);
    }
}

void J3DShapeTable::initShapeNodes(J3DDrawMtxData* pMtxData, J3DVertexData* pVtxData) {
    u16 shapeNum = mShapeNum;
    for (u16 i = 0; i < shapeNum; i++) {
        J3DShape* shapeNode = mShapeNodePointer[i];

        shapeNode->setDrawMtxDataPointer(pMtxData);
        shapeNode->setVertexDataPointer(pVtxData);
        shapeNode->makeVcdVatCmd();
    }
}

void J3DShapeTable::sortVcdVatCmd() {
    u16 shapeNum = mShapeNum;
    for (u16 next = 0; next < shapeNum; next++) {
        for (u16 prev = 0; prev < next; prev++) {
            if (mShapeNodePointer[next]->isSameVcdVatCmd(mShapeNodePointer[prev])) {
                mShapeNodePointer[next]->setVcdVatCmd(mShapeNodePointer[prev]->getVcdVatCmd());
            }
        }
    }
}
