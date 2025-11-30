#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStage/JSGFog.h"
#include "math.h"

JStage::TFog::~TFog() {}

s32 JStage::TFog::JSGFGetType() const {
    return 6;
}

bool JStage::TFog::JSGGetFogFunction() const {
    return false;
}

void JStage::TFog::JSGSetFogFunction(GXFogType) {}

f32 JStage::TFog::JSGGetStartZ() const {
    return 0.0f;
}

void JStage::TFog::JSGSetStartZ(f32) {}

f32 JStage::TFog::JSGGetEndZ() const {
    return FLT_MAX;
}

void JStage::TFog::JSGSetEndZ(f32) {}

GXColor JStage::TFog::JSGGetColor() const {
    return (GXColor){255, 255, 255, 255};
}

void JStage::TFog::JSGSetColor(GXColor) {}
