#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStage/JSGSystem.h"

JStage::TSystem::~TSystem() {}

s32 JStage::TSystem::JSGFGetType() const {
    return 1;
}

int JStage::TSystem::JSGFindObject(JStage::TObject**, char const*, JStage::TEObject) const {
    return 1;
}

bool JStage::TSystem::JSGCreateObject(char const*, JStage::TEObject, u32) {
    return false;
}

void JStage::TSystem::JSGDestroyObject(JStage::TObject*) {}

bool JStage::TSystem::JSGGetSystemData(u32) {
    return false;
}

void JStage::TSystem::JSGSetSystemData(u32, u32) {}
