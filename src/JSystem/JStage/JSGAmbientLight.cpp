#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStage/JSGAmbientLight.h"

JStage::TAmbientLight::~TAmbientLight() {}

s32 JStage::TAmbientLight::JSGFGetType() const {
    return 4;
}

GXColor JStage::TAmbientLight::JSGGetColor() const {
    return (GXColor){255, 255, 255, 255};
}

void JStage::TAmbientLight::JSGSetColor(GXColor) {}
