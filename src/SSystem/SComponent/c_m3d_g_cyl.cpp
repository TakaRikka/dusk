/**
 * c_m3d_g_cyl.cpp
 *
 */

#include "SSystem/SComponent/c_m3d_g_cyl.h"
#include "SSystem/SComponent/c_m3d.h"

#include "JSystem/JUtility/JUTAssert.h"

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

void cM3dGCyl::SetC(const cXyz& pos) {
    JUT_ASSERT(67, !isnan(pos.x));
    JUT_ASSERT(68, !isnan(pos.y));
    JUT_ASSERT(69, !isnan(pos.z));

    JUT_ASSERT(72, -1.0e32f < pos.x && pos.x < 1.0e32f && -1.0e32f < pos.y && pos.y < 1.0e32f && -1.0e32f < pos.z && pos.z < 1.0e32f);

    mCenter = pos;
}

void cM3dGCyl::SetH(f32 h) {
    JUT_ASSERT(82, !isnan(h));
    JUT_ASSERT(83, -1.0e32f < h && h < 1.0e32f);
    mHeight = h;
}

void cM3dGCyl::SetR(f32 r) {
    JUT_ASSERT(106, !isnan(r));
    JUT_ASSERT(107, -1.0e32f < r && r < 1.0e32f);
    mRadius = r;
}

bool cM3dGCyl::cross(const cM3dGSph* other, cXyz* out) const {
    return cM3d_Cross_CylSph(this, other, out);
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
