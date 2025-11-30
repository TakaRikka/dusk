#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStage/JSGObject.h"

JStage::TObject::~TObject() {}


bool JStage::TObject::JSGGetName() const {
    return false;
}

void JStage::TObject::JSGUpdate() {}

u32 JStage::TObject::JSGGetFlag() const {
    return 0;
}

void JStage::TObject::JSGSetFlag(u32) {}

bool JStage::TObject::JSGGetData(u32, void*, u32) const {
    return false;
}

void JStage::TObject::JSGSetData(u32, void const*, u32) {}

void JStage::TObject::JSGGetParent(JStage::TObject**, u32*) const {}

void JStage::TObject::JSGSetParent(JStage::TObject*, u32) {}

void JStage::TObject::JSGSetRelation(bool, JStage::TObject*, u32) {}

s32 JStage::TObject::JSGFindNodeID(char const*) const {
    return -1;
}

bool JStage::TObject::JSGGetNodeTransformation(u32, MtxP mtx) const {
    mtx[0][0] = 0.0f;
    mtx[0][1] = 0.0f;
    mtx[0][2] = 0.0f;
    mtx[0][3] = 0.0f;
    mtx[1][0] = 0.0f;
    mtx[1][1] = 0.0f;
    mtx[1][2] = 0.0f;
    mtx[1][3] = 0.0f;
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = 0.0f;
    mtx[2][3] = 0.0f;
    return false;
}
