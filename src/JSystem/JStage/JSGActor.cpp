#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStage/JSGActor.h"

JStage::TActor::~TActor() {}

s32 JStage::TActor::JSGFGetType() const {
    return 2;
}

void JStage::TActor::JSGGetTranslation(Vec*) const {}

void JStage::TActor::JSGSetTranslation(Vec const&) {}

void JStage::TActor::JSGGetScaling(Vec*) const {}

void JStage::TActor::JSGSetScaling(Vec const&) {}

void JStage::TActor::JSGGetRotation(Vec*) const {}

void JStage::TActor::JSGSetRotation(Vec const&) {}

s32 JStage::TActor::JSGGetShape() const {
    return -1;
}

void JStage::TActor::JSGSetShape(u32) {}

s32 JStage::TActor::JSGGetAnimation() const {
    return -1;
}

void JStage::TActor::JSGSetAnimation(u32) {}

f32 JStage::TActor::JSGGetAnimationFrame() const {
    return 0.0f;
}

void JStage::TActor::JSGSetAnimationFrame(f32) {}

f32 JStage::TActor::JSGGetAnimationFrameMax() const {
    return 0.0f;
}

f32 JStage::TActor::JSGGetAnimationTransition() const {
    return 0.0f;
}

void JStage::TActor::JSGSetAnimationTransition(f32) {}

s32 JStage::TActor::JSGGetTextureAnimation() const {
    return -1;
}

void JStage::TActor::JSGSetTextureAnimation(u32) {}

f32 JStage::TActor::JSGGetTextureAnimationFrame() const {
    return 0.0f;
}

void JStage::TActor::JSGSetTextureAnimationFrame(f32) {}

f32 JStage::TActor::JSGGetTextureAnimationFrameMax() const {
    return 0.0f;
}
