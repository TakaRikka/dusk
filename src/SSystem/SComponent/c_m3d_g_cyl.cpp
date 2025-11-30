/**
 * c_m3d_g_cyl.cpp
 *
 */

#include "SSystem/SComponent/c_m3d_g_cyl.h"
#include "SSystem/SComponent/c_m3d.h"

cM3dGCyl::cM3dGCyl(const cXyz* center, f32 radius, f32 height) {
    SetC(*center);
    SetR(radius);
    SetH(height);
}

void cM3dGCyl::Set(const cM3dGCylS& other) {
    SetC(cXyz(other.mCenter));
    SetR(other.mRadius);
    SetH(other.mHeight);
}

void cM3dGCyl::Set(const cXyz& center, f32 radius, f32 height) {
    SetC(center);
    SetR(radius);
    SetH(height);
}

void cM3dGCyl::SetC(const cXyz& center) {
    mCenter = center;
}

void cM3dGCyl::SetH(f32 height) {
    mHeight = height;
}

void cM3dGCyl::SetR(f32 radius) {
    mRadius = radius;
}

bool cM3dGCyl::cross(const cM3dGSph* other, cXyz* out) const {
    f32 f;
    return cM3d_Cross_CylSph(this, other, out, &f);
}

bool cM3dGCyl::cross(const cM3dGCyl* other, cXyz* out) const {
    return cM3d_Cross_CylCyl(this, other, out);
}

void cM3dGCyl::calcMinMax(cXyz* min, cXyz* max) {
    min->x = mCenter.x - mRadius;
    min->y = mCenter.y;
    min->z = mCenter.z - mRadius;
    max->x = mCenter.x + mRadius;
    max->y = mCenter.y + mHeight;
    max->z = mCenter.z + mRadius;
}
