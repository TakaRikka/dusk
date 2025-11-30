#include "JSystem/JSystem.h" // IWYU pragma: keep

#include "JSystem/JStage/JSGCamera.h"
#include "math.h"
#include "limits.h"

JStage::TCamera::~TCamera() {}

s32 JStage::TCamera::JSGFGetType() const {
    return 3;
}

bool JStage::TCamera::JSGGetProjectionType() const {
    return true;
}

void JStage::TCamera::JSGSetProjectionType(JStage::TECameraProjection) {}

f32 JStage::TCamera::JSGGetProjectionNear() const {
    return 0.0f;
}

void JStage::TCamera::JSGSetProjectionNear(f32) {}

f32 JStage::TCamera::JSGGetProjectionFar() const {
    return std::numeric_limits<f32>::max();
}

void JStage::TCamera::JSGSetProjectionFar(f32) {}

f32 JStage::TCamera::JSGGetProjectionFovy() const {
    return 0.0f;
}

void JStage::TCamera::JSGSetProjectionFovy(f32) {}

f32 JStage::TCamera::JSGGetProjectionAspect() const {
    return 0.0f;
}

void JStage::TCamera::JSGSetProjectionAspect(f32) {}

void JStage::TCamera::JSGGetProjectionField(f32*) const {}

void JStage::TCamera::JSGSetProjectionField(f32 const*) {}

bool JStage::TCamera::JSGGetViewType() const {
    return true;
}

void JStage::TCamera::JSGSetViewType(JStage::TECameraView) {}

void JStage::TCamera::JSGGetViewPosition(Vec*) const {}

void JStage::TCamera::JSGSetViewPosition(Vec const&) {}

void JStage::TCamera::JSGGetViewUpVector(Vec*) const {}

void JStage::TCamera::JSGSetViewUpVector(Vec const&) {}

void JStage::TCamera::JSGGetViewTargetPosition(Vec*) const {}

void JStage::TCamera::JSGSetViewTargetPosition(Vec const&) {}

f32 JStage::TCamera::JSGGetViewRoll() const {
    return 0.0f;
}

void JStage::TCamera::JSGSetViewRoll(f32) {}
